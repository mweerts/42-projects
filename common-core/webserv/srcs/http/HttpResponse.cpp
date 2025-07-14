#include "HttpResponse.hpp"

#include <sstream>

#include "Logger.hpp"

HttpResponse::HttpResponse() {
    _statusCode = HTTP_OK;
    _version = "HTTP/1.1";
    _content = "";
    _contentLength = 0;
    _contentType = "text/html";
    _serverName = "webserv42";
}

HttpResponse::~HttpResponse() {}

void HttpResponse::setStatusCode(StatusCode statusCode) {
    _statusCode = statusCode;
}

void HttpResponse::setHeader(const std::string& key, const std::string& value) {
    _headers[key] = value;
}

void HttpResponse::setContent(const std::string& content) {
    _content = content;
    _contentLength = content.length();
}

void HttpResponse::setContentType(const std::string& contentType) {
    _contentType = contentType;
}

StatusCode HttpResponse::getStatusCode() const {
    return _statusCode;
}

std::string HttpResponse::getServerName() const {
    return _serverName;
}

std::string HttpResponse::toString() const {
    std::string        response;
    std::ostringstream oss;
    oss << _statusCode;
    response += _version + " " + oss.str() + " " +
                GetHttpStatusText(_statusCode) + "\r\n";

    std::ostringstream contentLengthStream;
    contentLengthStream << _contentLength;
    response += "Server: " + _serverName + "\r\n";
    response += "Content-Type: " + _contentType + "\r\n";
    response += "Content-Length: " + contentLengthStream.str() + "\r\n";
    for (std::map<std::string, std::string>::const_iterator it =
             _headers.begin();
         it != _headers.end(); ++it) {
        response += it->first + ": " + it->second + "\r\n";
    }
    response += "\r\n";    // End of headers
    response += _content;  // Add the content
    return response;
}