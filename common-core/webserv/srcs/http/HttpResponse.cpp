#include "HttpResponse.hpp"

#include <sys/time.h>

#include <sstream>

#include "Logger.hpp"

HttpResponse::HttpResponse() {
    _statusCode = HTTP_OK;
    _version = "HTTP/1.1";
    _content = "";
    _contentLength = 0;
    _contentType = "text/html";
    _serverName = "webserv42";
    _date = "";
    _connection = "keep-alive";  // HTTP/1.1 default is keep-alive. HTTP/1.0
                                 // default is close
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
    _contentLength = content.size();
}

void HttpResponse::setContentType(const std::string& contentType) {
    _contentType = contentType;
}

void HttpResponse::setConnection(const std::string& connection) {
    _connection = connection;
}

void HttpResponse::setDate(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    struct tm* tm_info = gmtime(&tv.tv_sec);
    char       buffer[50];

    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", tm_info);
    const_cast<std::string&>(_date) = buffer;
}

StatusCode HttpResponse::getStatusCode() const {
    return _statusCode;
}

std::string HttpResponse::getServerName() const {
    return _serverName;
}

std::string HttpResponse::getConnection() const {
    return _connection;
}

std::string HttpResponse::toString() {
    std::string        response;
    std::ostringstream oss;
    std::ostringstream contentLengthStream;

    oss << _statusCode;
    response += _version + " " + oss.str() + " " +
                GetHttpStatusText(_statusCode) + "\r\n";
    contentLengthStream << _contentLength;
    setDate();

    response += "Server: " + _serverName + "\r\n";
    response += "Date: " + _date + "\r\n";
    response += "Content-Type: " + _contentType + "\r\n";
    response += "Content-Length: " + contentLengthStream.str() + "\r\n";
    response += "Connection: " + _connection + "\r\n";

    for (std::map<std::string, std::string>::const_iterator it =
             _headers.begin();
         it != _headers.end(); ++it) {
        response += it->first + ": " + it->second + "\r\n";
    }
    response += "\r\n";    // End of headers
    response += _content;  // Add the content
    return response;
}