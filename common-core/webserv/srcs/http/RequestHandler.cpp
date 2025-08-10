#include "RequestHandler.hpp"

#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>

#include "../handlers/cgi_handler.hpp"
#include "../parsing/GlobalConfig.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Logger.hpp"
#include "MimeTypes.hpp"
#include "http_status_code.hpp"
#include "utils.hpp"

// TODO create upload dir if not exists

RequestHandler::RequestHandler(const HttpRequest&  request,
                               const ServerConfig& serverConfig)
    : _serverConfig(serverConfig),
      _request(request),
      _cgiHandler(NULL),
      _rootPath(serverConfig.getRoot()),
      _autoindex(serverConfig.getAutoIndex()) {};

RequestHandler::~RequestHandler() {
    if (_cgiHandler) {
        _cgiHandler->cleanupAsyncCgi();
        delete _cgiHandler;
        _cgiHandler = NULL;
    }
}

void RequestHandler::setResponse(const HttpResponse& response) {
    _response = response;
}

const std::string RequestHandler::getRootPath() const {
    return _rootPath;
}

static std::string getLastModifiedTime(const std::string& path) {
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) == 0) {
        char       buffer[80];
        struct tm* tm_info = gmtime(&fileInfo.st_mtime);
        strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", tm_info);
        return std::string(buffer);
    }
    return "";
}

bool RequestHandler::shouldCloseConnection() const {
    if (_response.getStatusCode() != HTTP_OK) {
        return true;
    }

    return !_request.shouldKeepAlive();
}

void RequestHandler::handleRequest() {
    processRequest();

    if (_request.shouldKeepAlive()) {
        _response.setConnection("keep-alive");
    } else {
        _response.setConnection("close");
    }

    if (_response.getStatusCode() != HTTP_OK &&
        _response.getStatusCode() != HTTP_MOVED_PERMANENTLY) {
        std::ostringstream oss;
        oss << _response.getStatusCode();
        const std::string* errorPage = _serverConfig.getErrorPage(oss.str());
        // Logger::debug() << "ERROR PAGE :" + *errorPage + "  " + _rootPath;
        if (errorPage) {
            std::ifstream file((_rootPath + "/" + *errorPage).c_str());
            if (file.fail()) {
                _response.setContent(GetHtmlErrorPage(_response));
                _response.setContentType("text/html");
                return;
            }
            std::ostringstream ss;
            ss << file.rdbuf();
            _response.setContent(ss.str());
            _response.setContentType(
                MimeTypes::getType((_rootPath + "/" + *errorPage).c_str()));
            _response.setLastModified(
                getLastModifiedTime(_rootPath + "/" + *errorPage));
        } else {
            _response.setContent(GetHtmlErrorPage(_response));
            _response.setContentType("text/html");
        }
        _response.setConnection("close");
    }
}

void RequestHandler::sendResponse(int socket_fd) {
    std::string responseString = _response.toString();
    Logger::debug() << "Sending response...";
    // Logger::debug() << responseString;
    send(socket_fd, responseString.c_str(), responseString.size(), 0);
}

void RequestHandler::processRequest() {
    const Location*    location = _serverConfig.getLocation(_request.getUri());
    const std::string& method = _request.getMethod();

    _internalUri = _request.getUri();
    Logger::debug() << "processing request for uri: " << _internalUri;
    if (location) {
        if (location->getReturn()) {
            _response.setStatusCode(HTTP_MOVED_PERMANENTLY);
            _response.setLocation("http://localhost:8080" +
                                  urlDecode(*location->getReturn()));
            _response.setContent(GetHtmlErrorPage(_response));
            _response.setContentType("text/html");
            return;
        } else if (location->getAlias()) {
            _internalUri =
                *location->getAlias() +
                _request.getUri().substr((location->getName()).length());
        } else if (*location->getRoot() != "./")
            _rootPath = *location->getRoot();
        _autoindex = location->getAutoIndex();
    } else
        _rootPath = _serverConfig.getRoot();

    if (method == "GET") {
        if (!location || (location && location->getMethodIsAllowed("GET")))
            processGetRequest();
        else {
            _response.setStatusCode(HTTP_METHOD_NOT_ALLOWED);
        }
    } else if (method == "POST") {
        if (!location || (location && location->getMethodIsAllowed("POST")))
            processPostRequest();
        else
            _response.setStatusCode(HTTP_METHOD_NOT_ALLOWED);
    } else if (method == "DELETE") {
        if (!location || (location && location->getMethodIsAllowed("DELETE")))
            processDeleteRequest();
        else
            _response.setStatusCode(HTTP_METHOD_NOT_ALLOWED);
    } else
        _response.setStatusCode(HTTP_NOT_IMPLEMENTED);
}

void RequestHandler::processGetRequest() {
    std::string     fullPath;
    const Location* location = _serverConfig.getLocation(_internalUri);

    fullPath = _rootPath + _internalUri;

    // Check if this is a CGI script
    CgiHandler tempCgiHandler(_request, &_serverConfig);
    if (tempCgiHandler.isCgiScript(_internalUri)) {
        // Create a new CgiHandler on the heap for async processing
        _cgiHandler = new CgiHandler(_request, &_serverConfig);
        if (_cgiHandler->startAsyncCgi(_internalUri)) {
            return;  // CGI started, let event loop handle it
        } else {
            delete _cgiHandler;
            _cgiHandler = NULL;
            _response.setStatusCode(HTTP_INTERNAL_SERVER_ERROR);
            return;
        }
    }
	
    if (isDirectory(fullPath)) {
        if (location && location->getIndex() &&
            isReadable(fullPath + "/" + *location->getIndex())) {
            fullPath += "/" + *location->getIndex();
        } else if (_serverConfig.getIndex() &&
                   isReadable(fullPath + "/" + *_serverConfig.getIndex())) {
            fullPath += "/" + *_serverConfig.getIndex();
        }
    }
    if (!pathExist(fullPath)) {
        _response.setStatusCode(HTTP_NOT_FOUND);
        return;
    }

    if (isDirectory(fullPath)) {
        if (_autoindex) {
            _response.setStatusCode(HTTP_OK);
            _response.setContent(getHtmlIndexPage(_rootPath, _internalUri));
            _response.setContentType("text/html");
            return;
        } else {
            _response.setStatusCode(HTTP_FORBIDDEN);
            return;
        }
    }
    if (!isReadable(fullPath) || !isFile(fullPath)) {
        _response.setStatusCode(HTTP_FORBIDDEN);
        return;
    }

    // Regular file handling
    std::ifstream file(fullPath.c_str());
    if (file.fail()) {
        _response.setStatusCode(HTTP_FORBIDDEN);
        return;
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    _response.setContent(ss.str());
    _response.setContentType(MimeTypes::getType(fullPath.c_str()));
    _response.setLastModified(getLastModifiedTime(fullPath));

    file.close();
}

void RequestHandler::processPostRequest() {
    // Check if this is a CGI script first
    CgiHandler tempCgiHandler(_request, &_serverConfig);
    if (tempCgiHandler.isCgiScript(_internalUri)) {
        // Create a new CgiHandler on the heap for async processing
        _cgiHandler = new CgiHandler(_request, &_serverConfig);
        if (_cgiHandler->startAsyncCgi(_internalUri)) {
            return;  // CGI started, let event loop handle it
        } else {
            delete _cgiHandler;
            _cgiHandler = NULL;
            _response.setStatusCode(HTTP_INTERNAL_SERVER_ERROR);
            return;
        }
    }

    // Regular file upload handling
    std::ofstream file;
    std::string   fullPath = _rootPath + _internalUri;

    if (pathExist(fullPath) && isFile(fullPath)) {
        _response.setStatusCode(HTTP_NO_CONTENT);
        return;
    } else {
        file.open(fullPath.c_str(), std::ofstream::out | std::ofstream::trunc);
        if (file.fail()) {
            _response.setStatusCode(HTTP_FORBIDDEN);
            return;
        }
        if (_request.getBodySize() == 0) {
            _response.setStatusCode(HTTP_NO_CONTENT);
            file.close();
            return;
        }
        file << _request.readBodyAll();
        file.close();
        _response.setStatusCode(HTTP_CREATED);
        return;
    }
}

void RequestHandler::processDeleteRequest() {
    std::string fullPath = _rootPath + _request.getUri();

    if (!pathExist(fullPath)) {
        _response.setStatusCode(HTTP_NOT_FOUND);
        return;
    }
    if (remove(fullPath.c_str()) != 0) {
        _response.setStatusCode(HTTP_INTERNAL_SERVER_ERROR);
        return;
    }
    _response.setStatusCode(HTTP_NO_CONTENT);
}

void RequestHandler::generateErrorResponse(StatusCode         status_code,
                                           const std::string& error_msg) {
    (void)error_msg;  // use this to pass the error message to the response
    _response.setStatusCode(status_code);
    _response.setContent(GetHtmlErrorPage(_response));
    _response.setContentType("text/html");
    _response.setConnection("close");
}

HttpResponse& RequestHandler::getResponse() {
    return _response;
}

bool RequestHandler::hasCgiRunning() const {
    return _cgiHandler != NULL && _cgiHandler->isProcessing();
}

bool RequestHandler::processCgi() {
    if (!_cgiHandler || !_cgiHandler->isProcessing()) {
        return false;  // No CGI running
    }
    
    // Check for timeout first
    if (_cgiHandler->isTimedOut()) {
        Logger::warning() << "CGI process timed out";
        _response.setStatusCode(HTTP_GATEWAY_TIMEOUT);
        _cgiHandler->cleanupAsyncCgi();
        delete _cgiHandler;
        _cgiHandler = NULL;
        return true;  // Processing complete (with error)
    }
    
    // Process one step of CGI I/O
    if (_cgiHandler->processCgiIO()) {
        // CGI I/O is complete, build the response
        _cgiHandler->buildCgiResponse(_response);
        _cgiHandler->cleanupAsyncCgi();
        delete _cgiHandler;
        _cgiHandler = NULL;
        return true;  // CGI processing is complete
    }
    
    return false;  // CGI still processing
}
