/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_connection.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 00:56:59 by llebugle          #+#    #+#             */
/*   Updated: 2025/06/30 00:57:00 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_connection.hpp"

#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <ctime>
#include <iostream>
#include <vector>
#include <sys/poll.h>
#include "Logger.hpp"
#include "../handlers/RequestHandler.hpp"
#include "http_status_code.hpp"
#include "lib/utils.hpp"

ClientConnection::ClientConnection(int                 socket_fd,
                                   const ServerConfig& server_config)
    : socket_fd_(socket_fd),
      server_config_(server_config),
      last_activity_(time(0)),
      request_parser_(NULL),
      request_handler_(NULL),
      state_(READING_REQUEST),
      is_closed_(false),
      read_stream_(),	
      write_stream_(),
      outBuf_(65536, 16384),
      inBuf_(65536, 16384),
      headerBuf_(),
      headerSent_(0),
      bodySent_(0),
      sendingHeaders_(false) {
    UpdateActivity();
    request_parser_ = new RequestParser(current_request_, server_config_);
    request_ready_ = false;
}

ClientConnection::~ClientConnection() {
    if (request_parser_) {
        request_parser_->cleanup();
        delete request_parser_;
        request_parser_ = NULL;
    }
    if (request_handler_) {
        delete request_handler_;
        request_handler_ = NULL;
    }

    Close();
}

bool ClientConnection::HandleEvent(short revents) {
    UpdateActivity();

    if (revents & (POLLHUP | POLLERR | POLLNVAL)) {
        Logger::debug() << "Client " << socket_fd_ << " disconnected";
        return false;
    }

    switch (state_) {
        case READING_REQUEST:
            if (revents & POLLIN) {
                return HandleRead();
            }
            break;

        case WRITING_RESPONSE:
            if (revents & POLLOUT) {
                return HandleWrite();
            }
            break;
            
        // check if all errors are handled when reaching this
        case ERROR: return false;
        default: return false;
    }

    if (ShouldClose()) {
        return false;
    }

    return true;
}

static bool checkSocketError(int socket_fd) {
    int       err = 0;
    socklen_t len = sizeof(err);
    getsockopt(socket_fd, SOL_SOCKET, SO_ERROR, &err, &len);
    return err;
}

bool ClientConnection::HandleRead() {
    ssize_t bytes_read = recv(socket_fd_, read_buffer_, BUFFER_SIZE - 1, 0);

    if (bytes_read < 0) {
        if (checkSocketError(socket_fd_)) {  // because we can't use errno here
            Logger::error() << "Socket error on client " << socket_fd_;
            return false;
        }
        return true;
    }

    if (bytes_read == 0) {
        Logger::debug() << "Client " << socket_fd_ << " closed connection";
        return false;
    }

    read_buffer_[bytes_read] = '\0';

    RequestParser::Status parse_status =
        request_parser_->parse(read_buffer_, bytes_read);

    switch (parse_status) {
        case RequestParser::ERROR: {
            StatusCode  status_code = request_parser_->getStatusCode();
            std::string status_message = request_parser_->getStatusMessage();
            Logger::error() << "Error parsing request: " << status_code << " "
                            << status_message;
            return false;
        }
        case RequestParser::NEED_MORE_DATA: {
            Logger::debug() << "Need more data";
            return true;
        }
        case RequestParser::COMPLETE: {
            Logger::debug() << "Request parsing complete";
            request_ready_ = true;

            request_handler_ =
                new RequestHandler(current_request_, server_config_);
            request_handler_->handleRequest();

            // Prepare for response phase. Do NOT send headers yet for CGI,
            // headers will be built after CGI completes.
            if (request_handler_->hasCgiRunning()) {
                headerBuf_.clear();
                headerSent_ = 0;
                bodySent_ = 0;
                sendingHeaders_ = false;
            } else {
                headerBuf_ = request_handler_->getResponse().headersToString();
                headerSent_ = 0;
                bodySent_ = 0;
                sendingHeaders_ = true;
                // If static file, open it for streaming
                if (request_handler_->isStaticFileResponse()) {
                    read_stream_.open(request_handler_->getStaticFilePath());
                }
            }

            state_ = WRITING_RESPONSE;
            return true;
        }
        default:
            Logger::error() << "Unknown parse result: " << parse_status;
            return false;
    }

    return true;
}

bool ClientConnection::HandleWrite() {
    // If CGI is running, wait for aux-FD events to complete it.
    if (request_handler_->hasCgiRunning()) {
        return true;
    }
    
    // First send headers partially
    if (sendingHeaders_ && headerSent_ < headerBuf_.size()) {
        const char* data = headerBuf_.c_str() + headerSent_;
        size_t      len  = headerBuf_.size() - headerSent_;
        ssize_t     n    = send(socket_fd_, data, len, 0);
        if (n < 0) {
            if (checkSocketError(socket_fd_)) {
                Logger::error() << "Send error on client " << socket_fd_;
                return false;
            }
            return true; // would block
        }
        headerSent_ += static_cast<size_t>(n);
        if (headerSent_ < headerBuf_.size()) {
            return true; // continue later
        }
        sendingHeaders_ = false;
        // Respect one-write-per-event: defer body send to next POLLOUT
        return true;
    }

    // If we have not prepared headers yet (e.g., CGI still running), do nothing
    if (headerBuf_.empty()) {
        return true;
    }

    // Non-static: send body one chunk per POLLOUT
    if (!sendingHeaders_ && !request_handler_->isStaticFileResponse()) {
        const std::string& body = request_handler_->getResponse().getContent();
        if (bodySent_ < body.size()) {
            const char* bodyPtr = body.c_str() + bodySent_;
            size_t      bodyLen = body.size() - bodySent_;
            ssize_t     n       = send(socket_fd_, bodyPtr, bodyLen, 0);
            if (n < 0) {
                if (checkSocketError(socket_fd_)) return false;
                return true;
            }
            if (n > 0) bodySent_ += static_cast<size_t>(n);
            // After one body write, return to respect one-op-per-event
            return true;
        }
    }

    // Static file streaming: send from buffer one chunk per POLLOUT
    if (!sendingHeaders_ && request_handler_->isStaticFileResponse()) {
        StreamBuffer& ob = read_stream_.outBuffer();
        if (ob.wantConsumer()) {
            ssize_t n = send(socket_fd_, ob.data(), ob.size(), 0);
            if (n < 0) {
                if (checkSocketError(socket_fd_)) return false;
                return true;
            }
            if (n > 0) ob.consume(static_cast<size_t>(n));
            // One write done for this event
            return true;
        }
        // If file is EOF and buffer drained, finish response
        if (read_stream_.isEof() && ob.empty()) {
            read_stream_.close();
            // fall through to finalization below
        }
    }

    // Finalization: only when headers sent and body sent or streaming finished
    bool headersDone = (!sendingHeaders_ && headerSent_ >= headerBuf_.size());
    bool nonStaticDone = (!request_handler_->isStaticFileResponse() && bodySent_ >= request_handler_->getResponse().getContent().size());
    bool staticDone = (request_handler_->isStaticFileResponse() && read_stream_.isEof() && read_stream_.outBuffer().empty());
    if (headersDone && (nonStaticDone || staticDone)) {
        Logger::debug() << "Response sent to client " << socket_fd_;
        if (request_handler_->shouldCloseConnection()) {
            Logger::debug() << "Connection should be closed";
            Close();
            return false;
        }
        delete request_handler_; request_handler_ = NULL;
        delete request_parser_;  request_parser_  = NULL;
        current_request_.reset();
        state_ = READING_REQUEST;
        request_parser_ = new RequestParser(current_request_, server_config_);
        request_ready_ = false;
        UpdateActivity();
    }
    return true;
}

void ClientConnection::Close() {
    if (is_closed_)
        return;

    if (close(socket_fd_) < 0)
        Logger::error() << "Failed to close socket: " << strerror(errno);

    is_closed_ = true;
    socket_fd_ = -1;
}

void ClientConnection::UpdateActivity() {
    last_activity_ = time(NULL);
}

bool ClientConnection::IsTimedOut(int timeout_seconds) const {
    if (current_request_.shouldKeepAlive()) {
        return (time(NULL) - last_activity_) > timeout_seconds;
    }
    return false;
}

// State Queries

bool ClientConnection::NeedsToRead() const {
    return state_ == READING_REQUEST;
}

bool ClientConnection::NeedsToWrite() const {
    return state_ == WRITING_RESPONSE;
}

bool ClientConnection::ShouldClose() const {
    return state_ == ERROR;
}

int ClientConnection::GetSocketFd() const {
    return socket_fd_;
}

ClientConnection::State ClientConnection::GetState() const {
    return state_;
}

void ClientConnection::GetAuxPollFds(std::vector<pollfd>& out) const {
    if (state_ == WRITING_RESPONSE && request_handler_ && request_handler_->isStaticFileResponse()) {
        if (read_stream_.wantsFileRead()) {
            pollfd p; p.fd = read_stream_.fileFd(); p.events = POLLIN; p.revents = 0; out.push_back(p);
        }
    }
    // Include CGI pipes too
    if (state_ == WRITING_RESPONSE && request_handler_ && request_handler_->hasCgiRunning()) {
        int inFd = request_handler_->getCgiInputPipe();
        int outFd = request_handler_->getCgiOutputPipe();
        if (inFd != -1) { pollfd p; p.fd = inFd; p.events = POLLOUT; p.revents = 0; out.push_back(p); }
        if (outFd != -1) { pollfd p; p.fd = outFd; p.events = POLLIN;  p.revents = 0; out.push_back(p); }
    }
}

bool ClientConnection::HandleAuxEvent(int fd, short revents) {
    if (request_handler_ && request_handler_->isStaticFileResponse()) {
        if (fd == read_stream_.fileFd() && (revents & POLLIN)) {
            (void)read_stream_.onFileReadable();
        }
    }
    if (request_handler_ && request_handler_->hasCgiRunning()) {
        bool done = request_handler_->handleCgiFdEvent(fd, revents);
        if (done) {
            // Prepare headers of CGI response and switch to header sending
            headerBuf_ = request_handler_->getResponse().headersToString();
            headerSent_ = 0;
            sendingHeaders_ = true;
        }
    }
    return true;
}
