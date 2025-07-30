#include "HttpRequest.hpp"

#include <cctype>   // pour isxdigit
#include <cstdlib>  // pour atoi

#include "Logger.hpp"

HttpRequest::HttpRequest() {
    _method = "";
    _uri = "";
    _version = "HTTP/1.1";
    _body = "";
    _request_tmp_file = "";
    _body_start = 0;
    _body_length = 0;
    _body_is_file = false;
    _body_read_pos = 0;
    _body_reader_initialized = false;
}

HttpRequest::HttpRequest(const HttpRequest& other) {
    *this = other;
}

HttpRequest& HttpRequest::operator=(const HttpRequest& other) {
    if (this != &other) {
        _method = other._method;
        _uri = other._uri;
        _version = other._version;
        _headers = other._headers;
        _request_tmp_file = other._request_tmp_file;
        _body = other._body;
        _body_start = other._body_start;
        _body_length = other._body_length;
        _body_is_file = other._body_is_file;
        _body_read_pos = other._body_read_pos;
        _body_reader_initialized = other._body_reader_initialized;
    }
    return *this;
}

HttpRequest::~HttpRequest() {
    if (_body_file_stream.is_open()) {
        _body_file_stream.close();
    }
}

void HttpRequest::setMethod(const std::string& method) {
    _method = method;
}

std::string urlDecode(const std::string& encoded) {
    std::string decoded;
    decoded.reserve(encoded.length());

    for (size_t i = 0; i < encoded.length(); ++i) {
        if (encoded[i] == '%' && i + 2 < encoded.length()) {
            char hex1 = encoded[i + 1];
            char hex2 = encoded[i + 2];

            if (isxdigit(hex1) && isxdigit(hex2)) {
                int value = 0;
                if (hex1 >= '0' && hex1 <= '9')
                    value = (hex1 - '0') * 16;
                else if (hex1 >= 'A' && hex1 <= 'F')
                    value = (hex1 - 'A' + 10) * 16;
                else if (hex1 >= 'a' && hex1 <= 'f')
                    value = (hex1 - 'a' + 10) * 16;

                if (hex2 >= '0' && hex2 <= '9')
                    value += (hex2 - '0');
                else if (hex2 >= 'A' && hex2 <= 'F')
                    value += (hex2 - 'A' + 10);
                else if (hex2 >= 'a' && hex2 <= 'f')
                    value += (hex2 - 'a' + 10);

                decoded += static_cast<char>(value);
                i += 2;
            } else {
                decoded += encoded[i];
            }
        } else if (encoded[i] == '+') {
            decoded += ' ';
        } else {
            decoded += encoded[i];
        }
    }

    return decoded;
}

void HttpRequest::setUri(const std::string& uri) {
    _uri = urlDecode(uri);
}

void HttpRequest::setVersion(const std::string& version) {
    _version = version;
}

void HttpRequest::setHeader(const std::string& key, const std::string& value) {
    _headers[key] = value;
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

const std::string& HttpRequest::getRequestFilepath() const {
    return _request_tmp_file;
}

void HttpRequest::setBody(const std::string& body) {
    _body = body;
    _body_is_file = false;
    _request_tmp_file.clear();
    _body_start = 0;
    _body_length = body.size();
    resetBodyReader();
}

void HttpRequest::setRequestFilepath(const std::string& filepath) {
    _request_tmp_file = filepath;
}

void HttpRequest::setBodyParams(const std::string& filepath, size_t start_pos,
                                size_t length) {
    _request_tmp_file = filepath;
    _body_start = start_pos;
    _body_length = length;
    _body_is_file = true;
}

bool HttpRequest::readBodyChunk(std::string& chunk, size_t max_bytes) const {
    chunk.clear();

    if (_body_is_file) {
        return readBodyChunkFromFile(chunk, max_bytes);
    } else {
        return readBodyChunkFromMemory(chunk, max_bytes);
    }
}

bool HttpRequest::readBodyChunkFromMemory(std::string& chunk,
                                          size_t       max_bytes) const {
    if (_body_read_pos >= _body.length()) {
        return false;
    }

    size_t remaining = _body.length() - _body_read_pos;
    size_t to_read = (max_bytes < remaining) ? max_bytes : remaining;

    chunk = _body.substr(_body_read_pos, to_read);
    _body_read_pos += to_read;

    return true;
}

bool HttpRequest::readBodyChunkFromFile(std::string& chunk,
                                        size_t       max_bytes) const {
    if (!_body_reader_initialized) {
        _body_file_stream.open(_request_tmp_file.c_str(), std::ios::binary);
        if (!_body_file_stream.is_open()) {
            Logger::error() << "Failed to open file: " << _request_tmp_file;
            return false;
        }
        _body_file_stream.seekg(_body_start);
        if (_body_file_stream.fail()) {
            Logger::error() << "Failed to seek to position " << _body_start
                            << " in file: " << _request_tmp_file;
            return false;
        }
        _body_read_pos = 0;
        _body_reader_initialized = true;
    }

    if (_body_read_pos >= _body_length) {
        return false;
    }

    size_t remaining = _body_length - _body_read_pos;
    size_t to_read = (max_bytes < remaining) ? max_bytes : remaining;

    std::vector<char> buffer(to_read);
    _body_file_stream.read(&buffer[0], to_read);

    if (_body_file_stream.fail() && !_body_file_stream.eof()) {
        Logger::error() << "Failed to read from file: " << _request_tmp_file;
        return false;
    }

    size_t bytes_read = _body_file_stream.gcount();

    if (bytes_read > 0) {
        chunk.assign(&buffer[0], bytes_read);
        _body_read_pos += bytes_read;
        return true;
    }

    return false;
}

/*
 * careful with this function, it will read the whole body into memory
 * potentially breaking if body is too large for a string
 */
std::string HttpRequest::readBodyAll() const {
    if (_body_read_pos > 0) {
        resetBodyReader();
    }

    if (_body_length > std::string().max_size()) {
        Logger::error() << "Body exceeds maximum string size";
        return "";
    }

    std::string result;
    result.reserve(_body_length);

    std::string chunk;
    while (readBodyChunk(chunk)) {
        result += chunk;
    }

    return result;
}

void HttpRequest::resetBodyReader() const {
    _body_read_pos = 0;
    _body_reader_initialized = false;

    if (_body_file_stream.is_open()) {
        _body_file_stream.close();
    }
}

size_t HttpRequest::getBodySize() const {
    return _body_length;
}

bool HttpRequest::hasMoreBody() const {
    if (_body_is_file) {
        return _body_read_pos < _body_length;
    } else {
        return _body_read_pos < _body.length();
    }
}

// Utility methods
size_t HttpRequest::getContentLength() const {
    const std::map<std::string, std::string>& headers = _headers;

    std::map<std::string, std::string>::const_iterator it;
    it = headers.find("Content-Length");
    if (it != headers.end()) {
        return atoi(it->second.c_str());
    }
    return 0;
}

std::string HttpRequest::getContentType() const {
    std::map<std::string, std::string>::const_iterator it;
    it = _headers.find("Content-Type");
    if (it != _headers.end()) {
        return it->second;
    }

    it = _headers.find("content-type");
    if (it != _headers.end()) {
        return it->second;
    }

    return "";
}

bool HttpRequest::shouldKeepAlive() const {
    std::map<std::string, std::string>::const_iterator it;
    it = _headers.find("Connection");
    if (it != _headers.end()) {
        if (it->second.find("close") != std::string::npos) {
            return false;
        }
        if (it->second.find("keep-alive") != std::string::npos) {
            return true;
        }
    }
    return (_version == "HTTP/1.1");
}

bool HttpRequest::isRequestChunked() const {
    std::map<std::string, std::string>::const_iterator it =
        _headers.find("Transfer-Encoding");
    if (it != _headers.end()) {
        return it->second.find("chunked") != std::string::npos;
    }

    it = _headers.find("transfer-encoding");
    if (it != _headers.end()) {
        return it->second.find("chunked") != std::string::npos;
    }

    return false;
}

void HttpRequest::reset() {
    _method.clear();
    _uri.clear();
    _version = "HTTP/1.1";
    _headers.clear();
    _body.clear();
    _request_tmp_file.clear();
    _body_start = 0;
    _body_length = 0;
    _body_is_file = false;
    _body_read_pos = 0;
    _body_reader_initialized = false;

    if (_body_file_stream.is_open()) {
        _body_file_stream.close();
    }
}

std::ostream& operator<<(std::ostream& os, const HttpRequest& request) {
    os << "===== Request =====" << std::endl;
    os << "Method: " << request.getMethod() << "\n";
    os << "URI: " << request.getUri() << "\n";
    os << "Version: " << request.getVersion() << "\n";
    std::map<std::string, std::string>::const_iterator it;
    for (it = request.getHeaders().begin(); it != request.getHeaders().end();
         it++) {
        os << it->first << ": " << it->second << "\n";
    }
    os << "Request file path: " << request.getRequestFilepath() << "\n";
    os << "===== End Request =====" << std::endl;
    return os;
}
