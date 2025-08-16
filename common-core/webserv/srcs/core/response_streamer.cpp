/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response_streamer.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 01:39:16 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/14 01:39:17 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response_streamer.hpp"

#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>

#include "../handlers/file_streaming.hpp"
#include "../http/HttpResponse.hpp"
#include "Logger.hpp"
#include "lib/stream_buffer.hpp"
#include "lib/utils.hpp"

// TODO:
// - Name "-2" (try again later) for Clarity

ResponseStreamer::ResponseStreamer()
    : state_(HEADERS),
      response_type_(REGULAR),
      header_buffer_(""),
      header_sent_(0),
      body_content_(""),
      body_sent_(0),
      read_stream_() {}

ResponseStreamer::~ResponseStreamer() {
    reset();
}

void ResponseStreamer::reset() {
    state_ = HEADERS;
    response_type_ = REGULAR;
    header_buffer_.clear();
    header_sent_ = 0;
    body_content_.clear();
    body_sent_ = 0;
    read_stream_.close();
}

// Pre-built response (404, JSON, etc.)
void ResponseStreamer::prepareResponse(HttpResponse& response) {
    reset();
    header_buffer_ = response.headersToString();
    body_content_ = response.getContent();
    response_type_ = REGULAR;
    header_sent_ = 0;
    body_sent_ = 0;
}

void ResponseStreamer::prepareStaticFile(HttpResponse&      response,
                                         const std::string& path) {
    if (!read_stream_.open(path)) {
        Logger::error() << "Failed to open file for streaming: " << path;
        state_ = COMPLETE;
        return;
    }
	
    // enough for now but will not scale with very large headers
    header_buffer_ = response.headersToString();
    header_sent_ = 0;

    state_ = HEADERS;
    response_type_ = FILE;
}

void ResponseStreamer::prepareCgiResponse(HttpResponse& response) {
    header_buffer_ = response.headersToString();
    header_sent_ = 0;

    body_content_ = response.getContent();
    body_sent_ = 0;

    state_ = HEADERS;
    response_type_ = CGI;
}

ssize_t ResponseStreamer::writeNextChunk(int socket_fd) {
    switch (state_) {
        case HEADERS: return writeHeaders(socket_fd);
        case BODY: return writeBody(socket_fd);
        case COMPLETE: return 0;
        default: {
            Logger::error() << "Invalid ResponseStreamer state";
            return -1;
        }
    }
}

bool ResponseStreamer::wantsFileRead() const {
    return response_type_ == FILE && state_ != COMPLETE &&
           !read_stream_.isEof() &&
           const_cast<FileReadStream&>(read_stream_).getBuffer().wantProducer();
}

// ==== Private Helpers ==== //

static ssize_t sendWrapper(int socket_fd, const char* data, size_t len) {
    if (!data || len == 0 || socket_fd < 0) {
        Logger::error() << "Invalid data or socket fd";
        return 0;
    }

    ssize_t n = send(socket_fd, data, len, MSG_NOSIGNAL);
    if (n == 0) {
        Logger::warning() << "Socket closed while sending";
        return -1;
    }
    if (n < 0) {
        if (lib::checkSocketError(socket_fd)) {
            Logger::error() << "Socket error while sending response";
            // we can't use errno so just for debugging purposes
            // Logger::error() << "Socket error while sending response: " <<
            // strerror(errno);
            return -1;
        }
        return -2;  // would block try again later
    }
    return n;
}

ssize_t ResponseStreamer::writeHeaders(int socket_fd) {
    if (header_sent_ >= header_buffer_.size()) {
        state_ = BODY;
        return 0;
    }

    const char* data = header_buffer_.c_str() + header_sent_;
    size_t      len = header_buffer_.size() - header_sent_;

    ssize_t n = sendWrapper(socket_fd, data, len);
    if (n > 0) {
        header_sent_ += static_cast<size_t>(n);
        if (header_sent_ >= header_buffer_.size())
            state_ = BODY;
    }
    return n;
}

ssize_t ResponseStreamer::writeStaticFileBody(int socket_fd) {
    StreamBuffer& buffer = read_stream_.getBuffer();

    if (read_stream_.isEof() && buffer.empty()) {
        state_ = COMPLETE;
        return 0;
    }

    if (buffer.empty()) {
        return 0;
    }

    ssize_t n = sendWrapper(socket_fd, buffer.data(), buffer.size());
    if (n > 0) {
        buffer.consume(n);
        if (read_stream_.isEof() && buffer.empty()) {
            state_ = COMPLETE;
        }
    }
    return n;
}

ssize_t ResponseStreamer::writeResponseBody(int socket_fd) {
    if (body_sent_ >= body_content_.size()) {
        state_ = COMPLETE;
        return 0;
    }

    const char* data = body_content_.c_str() + body_sent_;
    size_t      len = body_content_.size() - body_sent_;

    ssize_t n = sendWrapper(socket_fd, data, len);
    if (n > 0) {
        body_sent_ += static_cast<size_t>(n);
        if (body_sent_ >= body_content_.size())
            state_ = COMPLETE;
    }
    return n;
}

ssize_t ResponseStreamer::writeBody(int socket_fd) {
    switch (response_type_) {
        case FILE: {
            return writeStaticFileBody(socket_fd);
        }
        case CGI:
        case REGULAR: {
            return writeResponseBody(socket_fd);
        }
        default: Logger::error() << "Invalid response type"; return -1;
    }
}

// ==== Public Helpers ==== //

ResponseStreamer::State ResponseStreamer::getState() const {
    return state_;
}

bool ResponseStreamer::isWritable() const {
    return state_ != COMPLETE;
}

bool ResponseStreamer::isComplete() const {
    return state_ == COMPLETE;
}

bool ResponseStreamer::isStreamingFile() const {
    return response_type_ == FILE;
}

bool ResponseStreamer::isStreamingCgi() const {
    return response_type_ == CGI;
}

FileReadStream& ResponseStreamer::getFileStream() const {
    return const_cast<FileReadStream&>(read_stream_);
}
