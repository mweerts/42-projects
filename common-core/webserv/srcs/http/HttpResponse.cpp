#include "HttpResponse.hpp"

#include <sys/time.h>

#include <sstream>

#include "Logger.hpp"

HttpResponse::HttpResponse() {
    _statusCode = HTTP_OK;
    _version = "HTTP/1.1";
    _serverName = "webserv42";
    _date = "";
    _contentType = "text/html";
    _contentLength = 0;
    _lastModified = "";
    _connection = "close";  // HTTP/1.1 default is keep-alive. HTTP/1.0
                                 // default is close
    _content = "";
}

HttpResponse::~HttpResponse() {}

void HttpResponse::setStatusCode(StatusCode statusCode) {
    _statusCode = statusCode;
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

void HttpResponse::setLastModified(const std::string& lastModified) {
    _lastModified = lastModified;
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

    if (!_serverName.empty())
        response += "Server: " + _serverName + "\r\n";
    if (!_date.empty())
        response += "Date: " + _date + "\r\n";
    if (!_contentType.empty())
        response += "Content-Type: " + _contentType + "\r\n";
    response += "Content-Length: " + contentLengthStream.str() + "\r\n";
    if (!_lastModified.empty())
        response += "Last-Modified: " + _lastModified + "\r\n";
    if (!_connection.empty())
        response += "Connection: " + _connection + "\r\n";

    response += "\r\n";
    response += _content;

    // Logger::debug() << "Response generated:\n" << response;
    return response;
}
