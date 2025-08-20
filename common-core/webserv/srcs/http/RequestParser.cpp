/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:34:32 by llebugle          #+#    #+#             */
/*   Updated: 2025/07/21 18:34:34 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"

#include <sys/stat.h>
#include <unistd.h>

#include <cstring>
#include <ctime>
#include <stdexcept>
#include <vector>

#include "../parsing/GlobalConfig.hpp"
#include "Logger.hpp"
#include "http_status_code.hpp"
#include "lib/file_utils.hpp"
#include "lib/utils.hpp"

size_t RequestParser::request_counter_ = 0;

RequestParser::RequestParser(HttpRequest& request, const ServerConfig& config)
    : request_(request), server_config_(config) {
    current_phase_ = REQUEST_LINE;
    status_code_ = HTTP_OK;
    status_message_.clear();
    headers_end_pos_ = 0;
    request_size_ = 0;
	req_buffer_.clear();
    req_filename_.clear();
}

RequestParser::~RequestParser() {
    cleanup();
}

void RequestParser::cleanup() {
    if (request_file_.is_open()) {
        request_file_.close();
    }

    // Comment this to keep the file for debugging
    if (!req_filename_.empty() && lib::pathExist(req_filename_)) {
        if (remove(req_filename_.c_str()) != 0) {
            Logger::warning() << "Failed to remove temporary request file: "
                              << req_filename_;
        }
    }
}

RequestParser::Status RequestParser::parse(const char* buffer,
                                           size_t      buffer_size) {
    if (!buffer || buffer_size == 0) {
        Logger::error() << "Invalid buffer data";
        setError(HTTP_BAD_REQUEST);
        return ERROR;
    }

	if (current_phase_ <= HEADERS && !hasBody()) {
		req_buffer_.append(buffer, buffer_size);
		if (req_buffer_.size() > HEADER_BUFFER_SIZE) {
			setError(HTTP_REQUEST_ENTITY_TOO_LARGE, "Request headers too large");
			return ERROR;
		}
	} else if (!saveToFile(buffer, buffer_size)) {
        Logger::error() << "Failed to save request data";
        setError(HTTP_INTERNAL_SERVER_ERROR);
        return ERROR;
    }

    if (current_phase_ <= HEADERS) {
        header_buffer_.append(buffer, buffer_size);
        // maybe add HEADER_BUFFER_SIZE to the config?
        if (header_buffer_.size() > HEADER_BUFFER_SIZE) {
            setError(HTTP_REQUEST_ENTITY_TOO_LARGE,
                     "Request headers too large");
            return ERROR;
        }
    }

    Status result = NEED_MORE_DATA;
    while (current_phase_ != COMPLETE_PHASE && current_phase_ != ERROR_PHASE) {
        switch (current_phase_) {
            case REQUEST_LINE: {
                result = parseRequestLine();
                if (result == COMPLETE) {
                    current_phase_ = HEADERS;
                } else if (result == ERROR) {
                    current_phase_ = ERROR_PHASE;
                    return ERROR;
                } else {
                    return NEED_MORE_DATA;
                }
                break;
            }

            case HEADERS: {
                result = parseHeaders();
                if (result == COMPLETE) {
                    current_phase_ = hasBody() ? BODY : COMPLETE_PHASE;
                } else if (result == ERROR) {
                    current_phase_ = ERROR_PHASE;
                    return ERROR;
                } else {
                    return NEED_MORE_DATA;
                }
                break;
            }

            case BODY: {
                result = parseBody();
                if (result == COMPLETE) {
                    current_phase_ = COMPLETE_PHASE;
                } else if (result == ERROR) {
                    current_phase_ = ERROR_PHASE;
                    return ERROR;
                } else {
                    return NEED_MORE_DATA;
                }
                break;
            }

            default: setError(HTTP_INTERNAL_SERVER_ERROR); return ERROR;
        }
    }

    return (current_phase_ == COMPLETE_PHASE) ? COMPLETE : ERROR;
}

std::string RequestParser::createRequestFilePath() {
    if (req_filename_.empty()) {
        std::string dir = server_config_.getRoot() + "/tmp";

        if (!lib::pathExist(dir)) {
            Logger::debug() << "Creating tmp directory for request: " << dir;
            if (mkdir(dir.c_str(), 0755) != 0) {
                throw std::runtime_error(
                    "Failed to create tmp directory for request");
            }
        } else if (!lib::isDirectory(dir)) {
            throw std::runtime_error("tmp path exists but is not a directory");
        } else if (!lib::isWritable(dir)) {
            throw std::runtime_error("tmp directory is not writable");
        }

        req_filename_ = dir + "/" + lib::to_string(time(NULL)) + "_" +
                        lib::to_string(request_counter_++);

        Logger::debug() << "Created tmp request file";
    }
    return req_filename_;
}

bool RequestParser::saveToFile(const char* buffer, size_t size) {
    if (!buffer || size == 0) {
        return false;
    }

    if (req_filename_.empty()) {
        try {
            req_filename_ = createRequestFilePath();
        } catch (const std::exception& e) {
            Logger::critical()
                << "Failed to create request file path: " << e.what();
            return false;
        }

        request_file_.open(req_filename_.c_str(),
                           std::ios::out | std::ios::trunc | std::ios::binary);
        if (!request_file_.is_open()) {
            Logger::error() << "Failed to open tmp request file";
            return false;
        }
        request_.setRequestFilepath(req_filename_);
    }

    request_file_.write(buffer, size);
    request_file_.flush();
    if (request_file_.fail()) {
        Logger::error() << "Failed to write to tmp request file";
        return false;
    }

    request_size_ += size;
    return true;
}

bool RequestParser::validateAndSetRequestLine(const std::string& line) {
    std::istringstream iss(line);
    std::string        method, uri, version, extra;

    iss >> method >> uri >> version;

    if (iss >> extra) {
        setError(HTTP_BAD_REQUEST, "Extra content in request line");
        return false;
    }
    if (method.empty() || uri.empty() || version.empty()) {
        setError(HTTP_BAD_REQUEST, "Missing words in request line");
        return false;
    }
    if (version != "HTTP/1.0" && version != "HTTP/1.1") {
        setError(HTTP_NOT_IMPLEMENTED, "Unsupported HTTP version");
        return false;
    }

    request_.setMethod(method);
    request_.setUri(uri);
    request_.setVersion(version);

    Logger::info() << "Request: " << method << " " << uri << " " << version;
    return true;
}

RequestParser::Status RequestParser::parseRequestLine() {
    size_t newline_pos = header_buffer_.find('\n');
    if (newline_pos == std::string::npos) {
        return NEED_MORE_DATA;
    }

    std::string request_line = header_buffer_.substr(0, newline_pos);
    if (!request_line.empty() &&
        request_line[request_line.size() - 1] == '\r') {
        request_line.erase(request_line.size() - 1);
    }

    if (!validateAndSetRequestLine(request_line)) {
        return ERROR;
    }

    return COMPLETE;
}

bool RequestParser::extractHeaders(const std::string& buffer) {
    std::istringstream iss(buffer);
    std::string        line;

    while (std::getline(iss, line) && !line.empty()) {
        if (!line.empty() && line[line.size() - 1] == '\r') {
            line.erase(line.size() - 1);
        }

        size_t pos = line.find(':');
        if (pos == std::string::npos) {
            setError(HTTP_BAD_REQUEST, "Invalid header format");
            return false;
        }

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        while (!value.empty() && value[0] == ' ') {
            value.erase(0, 1);
        }

        request_.setHeader(key, value);
    }

    return true;
}

RequestParser::Status RequestParser::parseHeaders() {
    Logger::debug() << "Parsing headers...";
    if (findHeadersEnd()) {
        size_t req_line_end = header_buffer_.find('\n');
        if (req_line_end == std::string::npos) {
            setError(HTTP_BAD_REQUEST, "No request line found");
            return ERROR;
        }

        size_t headers_start_pos = req_line_end + 1;
        size_t headers_length = headers_end_pos_ - headers_start_pos - 3;

        std::string headers_only =
            header_buffer_.substr(headers_start_pos, headers_length);

        if (!extractHeaders(headers_only)) {
            return ERROR;
        }

        Logger::debug() << "Headers parsed successfully";
        return COMPLETE;
    }
    return NEED_MORE_DATA;
}

bool RequestParser::hasBody() const {
    const std::map<std::string, std::string>& headers = request_.getHeaders();

    std::map<std::string, std::string>::const_iterator it;

    it = headers.find("Content-Length");
    if (it != headers.end() && std::atoi(it->second.c_str()) > 0) {
        return true;
    }

    it = headers.find("Transfer-Encoding");
    if (it != headers.end() &&
        it->second.find("chunked") != std::string::npos) {
        return true;
    }
    return false;
}

RequestParser::Status RequestParser::parseBody() {
    size_t content_length = request_.getContentLength();

    if (content_length > server_config_.getClientMaxBodySize()) {
        setError(HTTP_REQUEST_ENTITY_TOO_LARGE, "body size exceeds limit");
        return ERROR;
    }

    if (content_length > 0) {
        if (request_size_ >= (headers_end_pos_ + content_length)) {
            if (content_length <= 8192) {  // Small body: store as string
                std::string body =
                    readFromFile(headers_end_pos_, content_length);
                request_.setBody(body);
            } else {
                request_.setBodyParams(req_filename_, headers_end_pos_,
                                       content_length);
            }

            Logger::debug()
                << "Body parsed successfully (" << content_length << " bytes)";
            return COMPLETE;
        } else {
            return NEED_MORE_DATA;
        }
    } else {
        Logger::debug() << "No body to parse";
        return COMPLETE;
    }
}

bool RequestParser::findHeadersEnd() {
    size_t header_end = header_buffer_.find("\r\n\r\n");
    if (header_end != std::string::npos) {
        headers_end_pos_ = header_end + 4;
        return true;
    }

    header_end = header_buffer_.find("\n\n");
    if (header_end != std::string::npos) {
        headers_end_pos_ = header_end + 2;
        return true;
    }

    return false;
}

// get the length of the file from the start position
static size_t calculateBodyLength(std::ifstream& file, size_t start_pos) {
    file.seekg(0, std::ios::end);
    std::streampos file_end = file.tellg();
    if (file_end == std::streampos(-1)) {
        Logger::error() << "Failed to get file size";
        return 0;
    }
    size_t file_size = static_cast<size_t>(file_end);
    return file_size - start_pos;
}

std::string RequestParser::readFromFile(size_t start_pos, size_t length) const {
    if (req_filename_.empty()) {
        return "";
    }

    std::ifstream file(req_filename_.c_str(), std::ios::binary);
    if (!file.is_open()) {
        Logger::error() << "Failed to open request file: " << req_filename_;
        return "";
    }

    if (length == 0) {
        length = calculateBodyLength(file, start_pos);
        if (length <= 0)
            return "";
    }

    file.seekg(start_pos, std::ios::beg);
    if (file.fail()) {
        Logger::error() << "Failed to seekg for file: " << req_filename_;
        return "";
    }

    // for small file, 8KB threshold
    if (length <= 8192) {
        std::vector<char> buffer(length);
        file.read(&buffer[0], length);

        if (file.fail() && !file.eof()) {
            Logger::error() << "Failed to read from file: " << req_filename_;
            return "";
        }

        size_t bytes_read = static_cast<size_t>(file.gcount());
        return std::string(&buffer[0], bytes_read);
    }

    // streaming approach for large files
    std::string content;
    content.reserve(length);

    const size_t      CHUNK_SIZE = 4096;
    std::vector<char> chunk(CHUNK_SIZE);  // better for binary files

    size_t remaining = length;
    while (remaining > 0) {
        size_t to_read = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        file.read(&chunk[0], to_read);

        size_t bytes_read = static_cast<size_t>(file.gcount());
        if (bytes_read == 0)
            break;

        content.append(&chunk[0], bytes_read);
        remaining -= bytes_read;

        if (file.eof())
            break;
    }

    if (file.fail() && !file.eof()) {
        Logger::error() << "Failed to read from file: " << req_filename_;
        return "";
    }

    return content;
}

StatusCode RequestParser::getStatusCode() const {
    return status_code_;
}

std::string RequestParser::getStatusMessage() const {
    return status_message_;
}

bool RequestParser::isComplete() const {
    return current_phase_ == COMPLETE_PHASE;
}

bool RequestParser::needMoreData() const {
    return current_phase_ != COMPLETE_PHASE && current_phase_ != ERROR_PHASE;
}

bool RequestParser::hasError() const {
    return current_phase_ == ERROR_PHASE;
}

const HttpRequest& RequestParser::getRequest() const {
    return request_;
}

void RequestParser::setError(StatusCode         status_code,
                             const std::string& message) {
    status_code_ = status_code;
    if (!message.empty()) {
        status_message_ = message;
    }
}
