/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:44:01 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/01 20:44:02 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi_handler.hpp"

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/poll.h>

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

#include "../http/HttpRequest.hpp"
#include "../http/HttpResponse.hpp"
#include "../parsing/GlobalConfig.hpp"
#include "Logger.hpp"
#include "lib/file_utils.hpp"
#include "lib/utils.hpp"
#include "cgi_process.hpp"

/* potential refactors
 * functions :
 * - processCgiIO
 * - buildCgiResponse
 * others :
*/

const int                          CgiHandler::CGI_TIMEOUT_SECONDS = 10;
std::map<std::string, std::string> CgiHandler::cgiBin_;
bool                               CgiHandler::cgiBinInitialized_ = false;
std::string                        CgiHandler::cgiBinPath_;
std::string                        CgiHandler::cgiScriptPath_;

CgiHandler::CgiHandler(const HttpRequest& request)
    : request_(request), serverConfig_(NULL), async_process_(NULL) {
    if (!cgiBinInitialized_) {
        setDefaultCgiBin();
        cgiBinInitialized_ = true;
    }
}

CgiHandler::CgiHandler(const HttpRequest&  request,
                       const ServerConfig* serverConfig)
    : request_(request), serverConfig_(serverConfig), async_process_(NULL) {
    if (!cgiBinInitialized_) {
        setDefaultCgiBin();
        cgiBinInitialized_ = true;
    }
}

CgiHandler::~CgiHandler() {
    // Do not auto-clean here; owner controls lifecycle via cleanupAsyncCgi()
}

void CgiHandler::initializeCgiBin(const CgiBin& cgiBin) {
    if (cgiBinInitialized_) {
        return;
    }

    cgiBin_.clear();
    cgiBinPath_ = cgiBin.getRoot();
    if (cgiBinPath_.empty()) {
        Logger::error() << "No path configured for cgi";
        return;
    }

    std::vector<std::string> extensions = cgiBin.getExt();
    std::vector<std::string> paths = cgiBin.getPath();

    Logger::debug() << "Initializing CGI bin...";

    if (extensions.empty() || paths.empty() ||
        extensions.size() != paths.size()) {
        Logger::error() << "Invalid cgiBin configuration";
        return;
    }

    for (size_t i = 0; i < extensions.size(); i++) {
        cgiBin_[extensions[i]] = paths[i];
        Logger::debug() << "CGI bin: " << extensions[i] << " -> " << paths[i];
    }

    cgiBinInitialized_ = true;
    Logger::debug() << "CGI bin initialized successfully";
}

bool CgiHandler::isCgiScript(const std::string& uri) {
    const std::string script_path = resolveScriptPath(uri);

    if (!lib::pathExist(script_path) || !lib::isFile(script_path))
        return false;

    std::string interp = getCgiInterpreter(script_path);
    if (!interp.empty()) {
        return lib::isExecutable(interp);
    }
    Logger::debug() << "Script path: " << script_path;
    // If no interpreter, the script itself must be executable.
    return lib::isExecutable(script_path);
}

/* ============ Environment ============ */

static char to_upper_char(char c) {
    return static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
}

std::string CgiHandler::buildQueryString() {
    return lib::extractQueryFromUri(request_.getUri());
}

const std::vector<std::string> CgiHandler::buildEnvironment() {
    std::vector<std::string> env;
    env.reserve(12);  // minimum env variables

    env.push_back("REQUEST_METHOD=" + request_.getMethod());
    env.push_back("QUERY_STRING=" + buildQueryString());
    env.push_back("CONTENT_LENGTH=" +
                  lib::to_string(request_.getContentLength()));
    env.push_back("CONTENT_TYPE=" + request_.getContentType());
    env.push_back("SCRIPT_NAME=" + request_.getUri());
    env.push_back("PATH_INFO=" + request_.getUri());
    env.push_back("SERVER_NAME=" +
                  (serverConfig_ ? serverConfig_->getServerName() : ""));
    std::string port =
        serverConfig_ ? lib::to_string(serverConfig_->getPort()) : "";
    env.push_back("SERVER_PORT=" + port);
    env.push_back("SERVER_PROTOCOL=HTTP/1.1");
    env.push_back("SERVER_SOFTWARE=webserv/1.0");
    env.push_back("UPLOADS_DIR=" +
                  (serverConfig_ ? serverConfig_->getUploadDir() : ""));

    const std::map<std::string, std::string>& headers = request_.getHeaders();
    for (std::map<std::string, std::string>::const_iterator it =
             headers.begin();
         it != headers.end(); ++it) {
        std::string key = it->first;
        std::transform(key.begin(), key.end(), key.begin(), to_upper_char);
        std::replace(key.begin(), key.end(), '-', '_');
        env.push_back("HTTP_" + key + "=" + it->second);
    }

    return env;
}


/* ============ Async core ============ */

bool CgiHandler::startAsyncCgi(const std::string& scriptUri) {
    if (async_process_) {
        Logger::warning() << "Async CGI already running";
        return false;
    }

    cgiScriptPath_ = resolveScriptPath(scriptUri);
    Logger::debug() << "Starting async CGI for " << cgiScriptPath_;

    async_process_ = new CgiProcess();
    if (!startCgiProcess(cgiScriptPath_)) {
        cleanupAsyncCgi();
        return false;
    }
    Logger::debug() << "Async CGI started pid="
                    << async_process_->getChildPid();
    return true;
}

bool CgiHandler::isAsyncCgiComplete() const {
    if (!async_process_ || !async_process_->isValid())
        return false;

    int   status;
    pid_t r = waitpid(async_process_->getChildPid(), &status, WNOHANG);
    if (r != async_process_->getChildPid())
        return false;

    // just in case
    return async_process_->getOutputBuffer().empty();
}

bool CgiHandler::isTimedOut() const {
    if (!async_process_ || !async_process_->isValid()) {
        Logger::warning() << "Invalid CGI process";
        return false;
    }

    return (time(NULL) - async_process_->getStartTime() > CGI_TIMEOUT_SECONDS);
}

void CgiHandler::cleanupAsyncCgi() {
    if (async_process_) {
        async_process_->cleanup();
        delete async_process_;
        async_process_ = NULL;
    }
}

bool CgiHandler::startCgiProcess(const std::string& scriptPath) {
    std::string work_dir;
    size_t      slash = scriptPath.find_last_of('/');
    if (slash != std::string::npos)
        work_dir = scriptPath.substr(0, slash);

    std::string interpreter = getCgiInterpreter(scriptPath);

    std::vector<std::string> args;
    if (!interpreter.empty())
        args.push_back(interpreter);
    args.push_back(scriptPath);

    std::vector<std::string> env = buildEnvironment();

    const std::string exec_path =
        interpreter.empty() ? scriptPath : interpreter;
    if (!async_process_->startProcess(exec_path, args, env, work_dir)) {
        Logger::error() << "Failed to start CGI process";
        return false;
    }
    return true;
}

// not convinced about this
bool CgiHandler::processCgiIO() {
    if (!async_process_ || !async_process_->isValid())
        return false;

    if (async_process_->getInputPipe() != -1 &&
        async_process_->getInputBuffer().empty()) {
        if (request_.getContentLength() > 0 || request_.hasMoreBody()) {
			// enought for simple cgi scripts
			// but will not scale for large bodies
            async_process_->setInputBuffer(request_.readBodyAll());
        }
    }

    async_process_->flushInputOnce();
    bool eof = async_process_->readOutputOnce();

    if (eof) {
        int status;
        async_process_->hasExited(&status);
        return true;
    }
    return false;
}

bool CgiHandler::handleFdEvent(int fd, short revents) {
    if (!async_process_ || !async_process_->isValid()) return true;

    int inFd  = async_process_->getInputPipe();
    int outFd = async_process_->getOutputPipe();

    // Write to CGI stdin when POLLOUT
    if (fd == inFd && (revents & POLLOUT)) {
        if (async_process_->getInputBuffer().empty()) {
            if (request_.getContentLength() > 0 || request_.hasMoreBody()) {
                // Simple: push all at once (can be improved to incremental)
                async_process_->setInputBuffer(request_.readBodyAll());
            } else {
                async_process_->setInputBuffer("");
            }
        }
        (void)async_process_->flushInputOnce();
        // Not complete yet; wait for stdout EOF
        return false;
    }

    // Read from CGI stdout when POLLIN
    if (fd == outFd && (revents & POLLIN)) {
        bool eof = async_process_->readOutputOnce();
        if (eof) {
            int status; (void)async_process_->hasExited(&status);
            return true; // Completed
        }
        return false;
    }

    // Treat HUP/ERR/NVAL on any pipe as completion for safety
    if ((fd == outFd || fd == inFd) && (revents & (POLLHUP | POLLERR | POLLNVAL))) {
        int status; (void)async_process_->hasExited(&status);
        return true;
    }
    return false;
}

// need to clean this up a bit too
static void parseCgiHeadersAndBody(const std::string& raw,
                                   HttpResponse&      response) {
    if (raw.empty()) {
        response.setStatusCode(HTTP_INTERNAL_SERVER_ERROR);
        return;
    }

    size_t headerEnd = raw.find("\r\n\r\n");
    size_t sepLen = 4;
    if (headerEnd == std::string::npos) {
        headerEnd = raw.find("\n\n");
        sepLen = 2;
    }

    std::string headers, body;
    if (headerEnd != std::string::npos) {
        headers = raw.substr(0, headerEnd);
        body = raw.substr(headerEnd + sepLen);
    } else {
        body = raw;
    }

    // Minimal header parsing
    std::istringstream hs(headers);
    std::string        line;
    bool               hasStatus = false;
    while (std::getline(hs, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (line.empty())
            continue;
        size_t p = line.find(':');
        if (p == std::string::npos)
            continue;
        std::string key = line.substr(0, p);
        std::string value = line.substr(p + 1);
        while (!value.empty() && value[0] == ' ') value.erase(0, 1);

        if (key == "Status") {
            size_t      sp = value.find(' ');
            std::string code =
                (sp == std::string::npos) ? value : value.substr(0, sp);
            response.setStatusCode(
                static_cast<StatusCode>(std::atoi(code.c_str())));
            hasStatus = true;
        } else if (key == "Content-Type") {
            response.setContentType(value);
        } else if (key == "Location") {
            response.setLocation(value);
        } else {
            // TODO: support setting arbitrary headers if HttpResponse supports
            // it
        }
    }

    if (!hasStatus) {
        response.setStatusCode(HTTP_OK);
    }
    response.setContent(body);
}

void CgiHandler::buildCgiResponse(HttpResponse& response) {
    parseCgiHeadersAndBody(async_process_->getOutputBuffer(), response);
}

/* ============ Public helpers ============ */

int CgiHandler::getInputPipe() const {
    return async_process_ ? async_process_->getInputPipe() : -1;
}
int CgiHandler::getOutputPipe() const {
    return async_process_ ? async_process_->getOutputPipe() : -1;
}
bool CgiHandler::isProcessing() const {
    return async_process_ != NULL && async_process_->isValid();
}

/* ============ Private helpers ============ */

std::string CgiHandler::resolveScriptPath(const std::string& uri) const {
    return cgiBinPath_ + uri;
}

const std::map<std::string, std::string>& CgiHandler::getCgiBin() const {
    return cgiBin_;
}

const std::string CgiHandler::getFileExtension(const std::string& filename) {
    size_t lastDot = filename.find_last_of('.');
    if (lastDot == std::string::npos)
        return "";
    return filename.substr(lastDot);
}

std::string CgiHandler::getCgiInterpreter(const std::string& scriptPath) {
    std::string extension = getFileExtension(scriptPath);
    std::map<std::string, std::string>::const_iterator it =
        cgiBin_.find(extension);
    if (it != cgiBin_.end()) {
        return it->second;
    }
    return "";
}

void CgiHandler::setDefaultCgiBin() {
    cgiBin_[".py"] = "/usr/bin/python3";
    cgiBin_[".sh"] = "/bin/bash";
    cgiBin_[".php"] = "/usr/bin/php-cgi";
}
