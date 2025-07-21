#include "RequestHandler.hpp"

#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>

#include "../parsing/GlobalConfig.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Logger.hpp"
#include "MimeTypes.hpp"
#include "utils.hpp"

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

RequestHandler::~RequestHandler() {}

// void RequestHandler::setServerConfig(const ServerConfig& config) {
//     _ServerConfig = config;
// }

void RequestHandler::setRequest(const HttpRequest& request) {
    _request = request;
}
void RequestHandler::setResponse(const HttpResponse& response) {
    _response = response;
}

void RequestHandler::parseFullRequest(const std::string& request) {
    std::istringstream iss(request);
    std::string        line;
    std::string        requestLine;
    std::string        headers;
    std::string        body;

    // Read the request line
    if (std::getline(iss, requestLine) && !requestLine.empty()) {
        parseRequestLine(requestLine);
    }

    // Read headers
    while (std::getline(iss, line) && !line.empty()) {
        headers += line + "\r\n";
    }
    parseHeaders(headers);

    // Read body if present
    if (std::getline(iss, body)) {
        parseBody(body);
    }
    Logger::debug() << "done parsing request";
}
void RequestHandler::parseRequestLine(const std::string& requestLine) {
    std::istringstream iss(requestLine);
    std::string        method, uri, version;
    iss >> method >> uri >> version;
    Logger::debug() << "Parsed request line: " << method << " " << uri << " "
                    << version;
    if (method.empty() || uri.empty() || version.empty()) {
        _response.setStatusCode(HTTP_BAD_REQUEST);
        return;
    }
    if (version != "HTTP/1.0" && version != "HTTP/1.1") {
        _response.setStatusCode(HTTP_VERSION_NOT_SUPPORTED);
        return;
    }
    _request.setMethod(method);
    _request.setUri(uri);
    _request.setVersion(version);
}

void RequestHandler::parseHeaders(const std::string& headers) {
    std::istringstream iss(headers);
    std::string        line;
    while (std::getline(iss, line) && !line.empty()) {
        size_t pos = line.find(':');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            _request.setHeader(key, value);
        }
    }
}

void RequestHandler::parseBody(const std::string& body) {
    if (!_request.getBody().empty()) {
        _request.setBody(body);
    } else {
        _response.setStatusCode(HTTP_BAD_REQUEST);
    }
}

const std::string RequestHandler::getRootPath() const {
    return _rootPath;
}

void RequestHandler::handleRequest(const std::string& request) {
    parseFullRequest(request);
    processRequest();
    if (_response.getStatusCode() != HTTP_OK) {
        _response.setContent(GetHtmlErrorPage(_response));
        _response.setContentType("text/html");
    }
}

void RequestHandler::sendResponse(int socket_fd) {
    std::string responseString = _response.toString();
    // Logger::debug() << "Sending response:\n" << responseString;
    send(socket_fd, responseString.c_str(), responseString.size(), 0);
    if (_response.getConnection() == "close") {
        close(socket_fd);
    }
}

void RequestHandler::processRequest() {
    const std::string& method = _request.getMethod();

    // if (_request.getHeaders().at("Connection") == "keep-alive" ||
    //     _request.getHeaders().at("Connection") == "close") {
    //     _response.setConnection(_request.getHeaders().at("Connection"));
    // }
    _response.setConnection("keep-alive");

    if (method == "GET") {
        processGetRequest();
    } else if (method == "POST") {
        processPostRequest();
    } else if (method == "DELETE") {
        processDeleteRequest();
    } else {
        _response.setStatusCode(HTTP_NOT_IMPLEMENTED);
    }
}

void RequestHandler::processGetRequest() {
    std::string fullPath = _rootPath + _request.getUri();

    if (!pathExist(fullPath)) {
        _response.setStatusCode(HTTP_NOT_FOUND);
        return;
    }
    if (isDirectory(fullPath)) {
        if (_serverConfig.getAutoIndex()) {
            _response.setStatusCode(HTTP_OK);
            _response.setContent(
                getHtmlIndexPage(_rootPath, _request.getUri()));
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

void RequestHandler::processPostRequest() {}

void RequestHandler::processDeleteRequest() {
    std::string fullPath = _rootPath + "uploads" + _request.getUri();

    if (!pathExist(fullPath)) {
        _response.setStatusCode(HTTP_NOT_FOUND);
        return;
    }
    if (isDirectory(fullPath)) {
        _response.setStatusCode(HTTP_FORBIDDEN);
        return;
    }
    if (remove(fullPath.c_str()) != 0) {
        _response.setStatusCode(HTTP_INTERNAL_SERVER_ERROR);
        return;
    }
    _response.setStatusCode(HTTP_OK);
    _response.setContent("File deleted successfully.");
}
