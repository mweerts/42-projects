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

#include <signal.h>
#include <sys/stat.h>
#include <time.h>

#include <algorithm>
#include <cstring>
#include <set>
#include <sstream>

#include "../parsing/GlobalConfig.hpp"
#include "Logger.hpp"
#include "cgi_process.cpp"
#include "lib/file_utils.hpp"
#include "lib/utils.hpp"

// static init
std::map<std::string, std::string> CgiHandler::cgiBin_;
bool                               CgiHandler::cgiBinInitialized_ = false;
std::string                        CgiHandler::cgiBinPath_;

CgiHandler::CgiHandler(const HttpRequest& request) : request_(request), async_process_(NULL) {
    if (!cgiBinInitialized_) {
        setDefaultCgiBin();
        cgiBinInitialized_ = true;
    }
}

CgiHandler::~CgiHandler() {}

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
    Logger::debug() << "CGI path: " << cgiBinPath_;

    std::vector<std::string> extensions = cgiBin.getExt();
    std::vector<std::string> paths = cgiBin.getPath();

    Logger::debug() << "Initializing CGI bin...";

    if (extensions.empty()) {
        Logger::error() << "No extensions found in cgiBin";
        return;
    }

    if (paths.empty()) {
        Logger::error() << "No interpreters found in cgiBin";
        return;
    }

    for (size_t i = 0; i < extensions.size(); i++) {
        cgiBin_[extensions[i]] = paths[i];
        Logger::debug() << "CGI bin initialized for extension: "
                        << extensions[i] << " with path: " << paths[i];
    }

    cgiBinInitialized_ = true;
    Logger::debug() << "CGI bin initialized successfully";
}

bool CgiHandler::isCgiScript(const std::string& uri) {
    const std::string script_path = cgiBinPath_ + uri;

    if (!lib::pathExist(script_path) || !lib::isExecutable(script_path)) {
        return false;
    }

    std::string ext = getFileExtension(script_path);
    std::map<std::string, std::string>::const_iterator it = cgiBin_.find(ext);
    if (it == cgiBin_.end()) {
        return false;
    }

    Logger::debug() << "isCgiScript: yes";
    return lib::isExecutable(it->second);
}


/* ASYNC PROCESS */

bool CgiHandler::startAsyncCgi(const std::string& scriptPath) {
    async_process_ = new CgiProcess();
    async_process_->startTime = time(NULL);
    
    if (!startCgiProcess(scriptPath)) {
        cleanupAsyncCgi();
        return false;
    }
    
    Logger::debug() << "Async CGI process started with PID: " << async_process_->childPid;
    return true;
}

bool CgiHandler::processAsyncCgi(HttpResponse& response) {
    if (!async_process_ || !async_process_->isValid) {
        Logger::error() << "Invalid async CGI process state";
        return false;
    }
    
    // Check timeout
    if (isTimedOut()) {
        Logger::warning() << "Async CGI process timed out";
        cleanupAsyncCgi();
        return false;
    }
    
    // Process I/O
    if (processCgiIO()) {
        // CGI completed successfully
        buildCgiResponse(response);
        cleanupAsyncCgi();
        return true;
    }
    
    return false;  // Still processing
}


/* probably need to be moved to cgi_process */
bool CgiHandler::startCgiProcess(const std::string& scriptPath) {
    // Create pipes
    int inputPipeFds[2] = {-1, -1};
    int outputPipeFds[2] = {-1, -1};
    
    if (pipe(inputPipeFds) == -1 || pipe(outputPipeFds) == -1) {
        Logger::error() << "Failed to create pipes for async CGI process";
        return false;
    }
    
    // Make pipes non-blocking
    fcntl(inputPipeFds[0], F_SETFL, O_NONBLOCK);
    fcntl(inputPipeFds[1], F_SETFL, O_NONBLOCK);
    fcntl(outputPipeFds[0], F_SETFL, O_NONBLOCK);
    fcntl(outputPipeFds[1], F_SETFL, O_NONBLOCK);
    
    // Fork process
    async_process_->childPid = fork();
    if (async_process_->childPid == -1) {
        Logger::error() << "Failed to fork async CGI process";
        return false;
    }
    
    if (async_process_->childPid == 0) {
        // Child process
        close(inputPipeFds[1]);   // Close write end
        close(outputPipeFds[0]);  // Close read end
        
        dup2(inputPipeFds[0], STDIN_FILENO);
        dup2(outputPipeFds[1], STDOUT_FILENO);
        
        close(inputPipeFds[0]);
        close(outputPipeFds[1]);
        
        // Change to script directory
        std::string scriptDir = scriptPath.substr(0, scriptPath.find_last_of('/'));
        if (!scriptDir.empty() && chdir(scriptDir.c_str()) != 0) {
            exit(1);
        }
        
        // Execute CGI script
        std::string interpreter = getCgiInterpreter(scriptPath);
        if (!interpreter.empty()) {
            execl(interpreter.c_str(), interpreter.c_str(), scriptPath.c_str(), NULL);
        } else {
            execl(scriptPath.c_str(), scriptPath.c_str(), NULL);
        }
        exit(1);  // If exec fails
    } else {
        // Parent process
        close(inputPipeFds[0]);   // Close read end
        close(outputPipeFds[1]);  // Close write end
        
        async_process_->inputPipe = inputPipeFds[1];   // Write to child
        async_process_->outputPipe = outputPipeFds[0]; // Read from child
        async_process_->isValid = true;
        
        return true;
    }
}

bool CgiHandler::processCgiIO() {
    // Read from CGI stdout
    char buffer[4096];
    ssize_t bytes_read = read(async_process_->outputPipe, buffer, sizeof(buffer));
    
    if (bytes_read > 0) {
        async_process_->outputBuffer.append(buffer, bytes_read);
    } else if (bytes_read == 0) {
        // EOF - CGI finished
        return true;
    } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
        Logger::error() << "Error reading from async CGI process";
        return false;
    }
    
    // Check if process is still running
    int status;
    pid_t result = waitpid(async_process_->childPid, &status, WNOHANG);
    if (result == async_process_->childPid) {
        // Process finished
        return true;
    }
    
    return false;  // Still processing
}

void CgiHandler::buildCgiResponse(HttpResponse& response) {
    if (async_process_->outputBuffer.empty()) {
        response.setStatusCode(HTTP_INTERNAL_SERVER_ERROR);
        return;
    }
    
    // Parse CGI output and set response
    parseCgiResponse(async_process_->outputBuffer, response);
}



void CgiHandler::cleanupAsyncCgi() {
    if (async_process_) {
        if (async_process_->isValid) {
            kill(async_process_->childPid, SIGKILL);
        }
        delete async_process_;
        async_process_ = NULL;
    }
}



/* HELPERS */

const std::map<std::string, std::string>& CgiHandler::getCgiBin() const {
    return cgiBin_;
}

const std::string CgiHandler::getFileExtension(const std::string& filename) {
    size_t lastDot = filename.find_last_of('.');
    if (lastDot == std::string::npos) {
        return "";
    }
    return filename.substr(lastDot);
}

std::string CgiHandler::getCgiInterpreter(const std::string& scriptPath) {
    std::string extension = getFileExtension(scriptPath);

    std::map<std::string, std::string>::const_iterator it;
    it = cgiBin_.find(extension);
    if (it != cgiBin_.end()) {
        Logger::debug() << "Found interpreter for " << extension << ": "
                        << it->second;
        return it->second;
    }

    Logger::debug() << "Extension " << extension << " not supported for CGI";
    return "";
}

void CgiHandler::setDefaultCgiBin() {
    cgiBin_[".py"] = "/usr/bin/python3";
    cgiBin_[".sh"] = "/bin/bash";
    cgiBin_[".php"] = "/usr/bin/php-cgi";
}

int CgiHandler::getInputPipe() const {
    return async_process_ ? async_process_->inputPipe : -1;
}

int CgiHandler::getOutputPipe() const {
    return async_process_ ? async_process_->outputPipe : -1;
}

bool CgiHandler::isProcessing() const {
    return async_process_ != NULL && async_process_->isValid;
}

bool CgiHandler::isTimedOut() const {
    if (!async_process_ || !async_process_->isValid) {
        return false;
    }
    return (time(NULL) - async_process_->startTime > CGI_TIMEOUT_SECONDS);
}














/* LEGACY CODE */

bool CgiHandler::executeCgiScript(const std::string& scriptPath,
                                  HttpResponse&      response) {
    std::string path = cgiBinPath_ + scriptPath;
    (void)response;

    int childPid = -1;
    int inputPipe = -1;
    int outputPipe = -1;

    if (!createProcess(path, childPid, inputPipe, outputPipe)) {
        Logger::error() << "Failed to create CGI process";
        return false;
    }

    // // Communicate with process
    // bool success = communicateWithProcess(inputPipe, outputPipe, response);
    bool success = true;

    // // Wait for process to finish
    // if (!waitForProcess(childPid)) {
    //     Logger::error() << "CGI process failed or timed out";
    //     cleanupProcess(childPid);
    //     return false;
    // }

    // cleanupProcess(childPid);

    if (success) {
        Logger::debug() << "CGI script executed successfully";
    } else {
        Logger::error() << "Failed to process CGI output";
    }

    return success;
}

// Step 7: Create process and set up communication
bool CgiHandler::createProcess(const std::string& scriptPath, int& childPid,
                               int& inputPipe, int& outputPipe) {
    // Create pipes for communication
    int inputPipeFds[2] = {-1, -1};
    int outputPipeFds[2] = {-1, -1};

    if (pipe(inputPipeFds) == -1) {
        Logger::error() << "Failed to create input pipe for CGI process";
        return false;
    }

    if (pipe(outputPipeFds) == -1) {
        Logger::error() << "Failed to create output pipe for CGI process";
        close(inputPipeFds[0]);
        close(inputPipeFds[1]);
        return false;
    }

    // Fork the process
    childPid = fork();
    if (childPid == -1) {
        Logger::error() << "Failed to fork CGI process";
        close(inputPipeFds[0]);
        close(inputPipeFds[1]);
        close(outputPipeFds[0]);
        close(outputPipeFds[1]);
        return false;
    }

    if (childPid == 0) {
        // Child process - execute CGI script
        // Close unused pipe ends
        close(inputPipeFds[1]);   // Close write end of input pipe
        close(outputPipeFds[0]);  // Close read end of output pipe

        // Redirect stdin/stdout
        if (dup2(inputPipeFds[0], STDIN_FILENO) == -1 ||
            dup2(outputPipeFds[1], STDOUT_FILENO) == -1) {
            exit(1);
        }

        // Close original pipe ends
        close(inputPipeFds[0]);
        close(outputPipeFds[1]);

        // Change to script directory (requirement 8)
        std::string scriptDir =
            scriptPath.substr(0, scriptPath.find_last_of('/'));
        if (!scriptDir.empty() && chdir(scriptDir.c_str()) != 0) {
            // Cannot use Logger in child process after fork
            exit(1);
        }

        // Prepare environment
        std::vector<std::string> envVars = buildEnvironment();
        std::vector<const char*> envArray;
        envArray.reserve(envVars.size() + 1);
        for (std::vector<std::string>::const_iterator it = envVars.begin();
             it != envVars.end(); ++it) {
            envArray.push_back(it->c_str());
        }
        envArray.push_back(NULL);  // NULL terminator

        // Get interpreter
        std::string interpreter = getCgiInterpreter(scriptPath);

        // Prepare arguments (requirement 7: file as first argument)
        std::vector<const char*> args;
        if (!interpreter.empty()) {
            args.reserve(3);
            args.push_back(interpreter.c_str());
        } else {
            args.reserve(2);
        }
        args.push_back(scriptPath.c_str());  // Script path as first argument
        args.push_back(NULL);                // NULL terminator

        // Execute the script
        if (!interpreter.empty()) {
            execve(interpreter.c_str(), const_cast<char* const*>(&args[0]),
                   const_cast<char* const*>(&envArray[0]));
        } else {
            execve(scriptPath.c_str(), const_cast<char* const*>(&args[0]),
                   const_cast<char* const*>(&envArray[0]));
        }

        // If we get here, exec failed
        exit(1);
    } else {
        // Parent process - set up communication
        close(inputPipeFds[0]);   // Close read end of input pipe
        close(outputPipeFds[1]);  // Close write end of output pipe

        inputPipe = inputPipeFds[1];    // Write to child's stdin
        outputPipe = outputPipeFds[0];  // Read from child's stdout

        return true;
    }
}

// // Step 5: Build query string from request
// std::string CgiHandler::buildQueryString() {
//     std::string uri = request_.getUri();
//     size_t      queryPos = uri.find('?');
//     if (queryPos == std::string::npos) {
//         return "";
//     }
//     return uri.substr(queryPos + 1);
// }

// // Step 6: Build environment variables for CGI script
const std::vector<std::string> CgiHandler::buildEnvironment() {
    std::vector<std::string> env;

    // Standard CGI environment variables
    env.push_back("REQUEST_METHOD=" + request_.getMethod());
    // env.push_back("QUERY_STRING=" + buildQueryString());
    env.push_back("CONTENT_LENGTH=" +
                  lib::to_string(request_.getContentLength()));
    env.push_back("CONTENT_TYPE=" + request_.getContentType());
    env.push_back("SCRIPT_NAME=" + request_.getUri());
    env.push_back("PATH_INFO=" + request_.getUri());  // Full path as requested 
	env.push_back("SERVER_NAME=localhost");
    env.push_back("SERVER_PORT=8080");  // Default port
    env.push_back("SERVER_PROTOCOL=HTTP/1.1");
    env.push_back("SERVER_SOFTWARE=webserv/1.0");

    // HTTP headers as environment variables
    const std::map<std::string, std::string>& headers = request_.getHeaders();
    for (std::map<std::string, std::string>::const_iterator it =
             headers.begin();
         it != headers.end(); ++it) {
        std::string envName = "HTTP_" + it->first;
        std::transform(envName.begin(), envName.end(), envName.begin(),
                       ::toupper);
        std::replace(envName.begin(), envName.end(), '-', '_');
        env.push_back(envName + "=" + it->second);
    }

    Logger::debug() << "Built " << env.size()
                    << " environment variables for CGI";
    return env;
}

// // Step 8: Communicate with the CGI process
// bool CgiHandler::communicateWithProcess(int inputPipe, int outputPipe,
//                                         HttpResponse& response) {
//     // Send request body to CGI script via stdin (requirement 5: handle
//     chunked) std::string requestBody = request_.getBody(); if
//     (!requestBody.empty()) {
//         ssize_t written =
//             write(inputPipe, requestBody.c_str(), requestBody.length());
//         if (written == -1) {
//             Logger::error() << "Failed to write to CGI process stdin";
//             close(inputPipe);
//             close(outputPipe);
//             return false;
//         }
//     }
//     close(inputPipe);  // Close stdin to signal EOF (requirement 5)

//     // Read output from CGI script (requirement 6: handle missing
//     // content-length)
//     std::string cgiOutput;
//     char        buffer[4096];
//     ssize_t     bytesRead;

//     while ((bytesRead = read(outputPipe, buffer, sizeof(buffer))) > 0) {
//         cgiOutput.append(buffer, bytesRead);
//     }

//     close(outputPipe);

//     Logger::debug() << "Read " << cgiOutput.length()
//                     << " bytes from CGI script";

//     // Parse CGI response
//     return parseCgiResponse(cgiOutput, response);
// }

// // Step 9: Parse CGI response
bool CgiHandler::parseCgiResponse(const std::string& cgiOutput,
                                  HttpResponse&      response) {
    if (cgiOutput.empty()) {
        Logger::error() << "Empty response from CGI script";
        return false;
    }

    // Split headers and body
    std::string headers, body;
    size_t      headerEnd = cgiOutput.find("\r\n\r\n");
    if (headerEnd == std::string::npos) {
        headerEnd = cgiOutput.find("\n\n");
    }

    if (headerEnd != std::string::npos) {
        headers = cgiOutput.substr(0, headerEnd);
        body = cgiOutput.substr(
            headerEnd +
            (cgiOutput.find("\r\n\r\n") != std::string::npos ? 4 : 2));
    } else {
        // No headers found, treat entire output as body
        body = cgiOutput;
    }

    // Parse headers
    std::istringstream headerStream(headers);
    std::string        line;
    bool               hasStatus = false;

    while (std::getline(headerStream, line)) {
        if (line.empty() || line == "\r")
            continue;

        // Remove carriage return if present
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);

            // Trim whitespace from value
            while (!value.empty() && value[0] == ' ') {
                value.erase(0, 1);
            }

            if (key == "Status") {
                // Parse status line (e.g., "Status: 200 OK")
                size_t spacePos = value.find(' ');
                if (spacePos != std::string::npos) {
                    std::string statusCode = value.substr(0, spacePos);
                    response.setStatusCode(
                        static_cast<StatusCode>(std::atoi(statusCode.c_str())));
                }
                hasStatus = true;
            } else if (key == "Content-Type") {
                response.setContentType(value);
            } else if (key == "Location") {
                response.setLocation(value);
            } else {
                // response.setHeader(key, value);
				// to implement setHeaders?
            }
        }
    }

    // Set default status if not provided
    if (!hasStatus) {
        response.setStatusCode(HTTP_OK);
    }

    // Set content
    response.setContent(body);

    Logger::debug() << "Parsed CGI response with " << body.length()
                    << " bytes of content";
    return true;
}

// // Step 10: Wait for process with timeout
// bool CgiHandler::waitForProcess(int pid) {
//     int    status;
//     time_t startTime = time(NULL);

//     while (time(NULL) - startTime < CGI_TIMEOUT_SECONDS) {
//         int result = waitpid(pid, &status, WNOHANG);
//         if (result == pid) {
//             // Process finished
//             if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
//                 return true;
//             } else {
//                 Logger::error() << "CGI script exited with error code: "
//                                 << WEXITSTATUS(status);
//                 return false;
//             }
//         } else if (result == -1) {
//             Logger::error() << "Error waiting for CGI process";
//             return false;
//         }
//         // Process still running, wait a bit
//         usleep(10000);  // 10ms
//     }

//     // Timeout reached
//     Logger::error() << "CGI script timed out after " << CGI_TIMEOUT_SECONDS
//                     << " seconds";
//     kill(pid, SIGKILL);
//     waitpid(pid, &status, 0);
//     return false;
// }

// // Step 11: Clean up process
// void CgiHandler::cleanupProcess(int pid) {
//     int status;
//     if (waitpid(pid, &status, WNOHANG) == 0) {
//         // Process still running, kill it
//         kill(pid, SIGKILL);
//         waitpid(pid, &status, 0);
//     }
// }

