
#include <sys/time.h> 
#include <sstream>

#include "../parsing/GlobalConfig.hpp"
#include "Logger.hpp"
#include "mime_types.hpp"
#include "http_utils.hpp"
#include "lib/file_utils.hpp"
#include "lib/utils.hpp"
#include "utils.hpp"
#include "http_response.hpp"
#include <fstream>

HttpResponse::HttpResponse(const ServerConfig& serverConfig)
    : _serverConfig(serverConfig) {
    _statusCode = HTTP_OK;
    _version = "HTTP/1.1";
    _serverName = "webserv42";
    _date = "";
    _contentType = "text/html";
    _contentLength = 0;
    _lastModified = "";
    _connection = "keep-alive";
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

void HttpResponse::setLocation(const std::string& location) {
    _location = location;
}

void HttpResponse::setContentLength(size_t length) {
    _contentLength = length;
}

const std::string& HttpResponse::getContent() const {
    return _content;
}

size_t HttpResponse::getContentLength() const {
    return _contentLength;
}

void HttpResponse::setHeader(const std::string& key, const std::string& value) {
    if (key.empty()) {
        Logger::debug() << "setHeader: key is empty";
        return;
    }
    _additionnalHeaders[key] = value;
}

void HttpResponse::removeAdditionnalHeader(const std::string& key) {
    _additionnalHeaders.erase(key);
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

const std::map<std::string, std::string>& HttpResponse::getAdditionnalHeaders()
    const {
    return _additionnalHeaders;
}

std::string HttpResponse::getAdditionnalHeader(const std::string& key) const {
    std::map<std::string, std::string>::const_iterator it =
        _additionnalHeaders.find(key);
    if (it == _additionnalHeaders.end()) {
        return "";
    }
    return it->second;
}

std::string HttpResponse::headersToString() {
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
    if (!_location.empty())
        response += "Location: " + _location + "\r\n";
    if (!_lastModified.empty())
        response += "Last-Modified: " + _lastModified + "\r\n";
    if (!_connection.empty())
        response += "Connection: " + _connection + "\r\n";

    std::map<std::string, std::string>::const_iterator it;
    for (it = _additionnalHeaders.begin(); it != _additionnalHeaders.end();
         it++) {
        response += it->first + ": " + it->second + "\r\n";
    }
    response += "\r\n";
    return response;
}

static void replaceErrorPlaceholders(std::string&        content,
                                     const HttpResponse& response) {
    std::string errCode = lib::to_string(response.getStatusCode());

    size_t pos = 0;
    while ((pos = content.find("{{ERROR_CODE}}")) != std::string::npos) {
        content.replace(pos, 14, errCode);
    }

    while ((pos = content.find("{{ERROR_MESSAGE}}")) != std::string::npos) {
        std::string errMessage = GetHttpStatusText(response.getStatusCode());
        content.replace(pos, 17, errMessage);
    }
}

void HttpResponse::CreateErrorPage(StatusCode statusCode) {
    _statusCode = statusCode;
    std::string err_page = _serverConfig.getErrorPage(statusCode);
    if (!err_page.empty()) {
        const char*   page = err_page.c_str();
        std::ifstream file(page);
        if (!file.fail() && file.is_open()) {
            std::ostringstream ss;
            ss << file.rdbuf();
            _content = ss.str();

            replaceErrorPlaceholders(_content, *this);
            _contentType = MimeTypes::getType(page);
            _contentLength = _content.size();
            _lastModified = lib::getLastModifiedTime(page);
            return;
        }
        Logger::error() << "Error page failed to open: " << err_page;
    }

    _contentType = "text/html";
    std::string msg = _content.empty() ? "" : "<h4>" + _content + "</h4>";
    std::ostringstream oss;
    oss << "<html><head><title>" << lib::to_string(_statusCode) << " "
        << GetHttpStatusText(_statusCode) << "</title></head><body><center><h1>"
        << lib::to_string(_statusCode) << " " << GetHttpStatusText(_statusCode)
        << "</h1>" << msg << "</center><hr><center>" << _serverName
        << "</center></body></html>";
    _content = oss.str();
    _contentLength = _content.size();
}

void HttpResponse::CreateErrorPage() {
    CreateErrorPage(_statusCode);
}

std::string HttpResponse::toString() {
    std::string response = headersToString();
    response += _content;
    return response;
}
