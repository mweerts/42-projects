#include "RequestHandler.hpp"

#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include <fstream>
#include <sstream>

#include "Logger.hpp"

static std::string GetHtmlErrorPage(HttpResponse& response) {
    std::ostringstream oss;
    oss << response.getStatusCode();
    return "<html><head><title>" + oss.str() + " " +
           GetHttpStatusText(response.getStatusCode()) +
           "</title></head><body><center><h1>" + oss.str() + " " +
           GetHttpStatusText(response.getStatusCode()) +
           "</h1></center><hr><center>" + response.getServerName() +
           "</center></body></html>";
}

static bool pathExist(const std::string& path) {
    struct stat fileInfo;
    return stat(path.c_str(), &fileInfo) == 0;
}

static bool isFile(const std::string& path) {
    struct stat fileInfo;
    stat(path.c_str(), &fileInfo);
    return S_ISREG(fileInfo.st_mode);
}

static bool isDirectory(const std::string& path) {
    struct stat fileInfo;
    stat(path.c_str(), &fileInfo);
    return S_ISDIR(fileInfo.st_mode);
}

static bool isReadable(const std::string& path) {
    struct stat fileInfo;
    stat(path.c_str(), &fileInfo);
    return S_IRUSR & fileInfo.st_mode && !access(path.c_str(), R_OK);
}

RequestHandler::RequestHandler() {}

RequestHandler::~RequestHandler() {}

void RequestHandler::setServerConfig(const ServerConfig& config) {
    _serverConfig = config;
}
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
    Logger::debug() << "Sending response:\n" << responseString;
    send(socket_fd, responseString.c_str(), responseString.size(), 0);
    if (_response.getConnection() == "close") {
        close(socket_fd);
    }
}

void RequestHandler::processRequest() {
    const std::string& method = _request.getMethod();

    if (_request.getHeaders().at("Connection") == "keep-alive" ||
        _request.getHeaders().at("Connection") == "close") {
        _response.setConnection(_request.getHeaders().at("Connection"));
    }

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
    if (!pathExist(_request.getUri())) {
        _response.setStatusCode(HTTP_NOT_FOUND);
        return;
    }
    if (isDirectory(_request.getUri())) {
        // Handle autoindex
        _response.setStatusCode(HTTP_FORBIDDEN);
        return;
    }
    if (!isReadable(_request.getUri()) || !isFile(_request.getUri())) {
        _response.setStatusCode(HTTP_FORBIDDEN);
        return;
    }
    std::ifstream file(_request.getUri().c_str());
    if (file.fail()) {
        _response.setStatusCode(HTTP_FORBIDDEN);
        return;
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    _response.setContent(ss.str());
    file.close();
}

void RequestHandler::processPostRequest() {}

void RequestHandler::processDeleteRequest() {}
