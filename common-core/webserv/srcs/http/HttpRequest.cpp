#include "HttpRequest.hpp"

#include "Logger.hpp"

HttpRequest::HttpRequest() {
    _method = "";
    _uri = "";
    _version = "";
    _body = "";
}

HttpRequest::~HttpRequest() {}

void HttpRequest::setMethod(const std::string& method) {
    _method = method;
}

void HttpRequest::setUri(const std::string& uri) {
    // use root from config here
    _uri = "/home/llebugle/Documents/webserv/www/" + uri;
}

void HttpRequest::setVersion(const std::string& version) {
    _version = version;
}

void HttpRequest::setHeader(const std::string& key, const std::string& value) {
    _headers[key] = value;
}

void HttpRequest::setBody(const std::string& body) {
    _body = body;
}

const std::string& HttpRequest::getMethod() const {
    return _method;
}

const std::string& HttpRequest::getUri() const {
    return _uri;
}

const std::string& HttpRequest::getVersion() const {
    return _version;
}

const std::map<std::string, std::string>& HttpRequest::getHeaders() const {
    return _headers;
}

const std::string& HttpRequest::getBody() const {
    return _body;
}