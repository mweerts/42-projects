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

#include "../handlers/RequestHandler.hpp"
#include "Logger.hpp"
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
      request_ready_(false) {
    UpdateActivity();
    request_parser_ = new RequestParser(current_request_, server_config_);
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
        if (state_ == WRITING_RESPONSE && !response_streamer_.isComplete()) {
            Logger::warning() << "Client disconnected before response complete";
        }
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
        case ERROR: return false;
        default: return false;
    }

    if (ShouldClose()) {
        return false;
    }

    return true;
}

bool ClientConnection::HandleRead() {
    if (request_ready_) {
        return true;
    }

    ssize_t bytes_read = recv(socket_fd_, read_buffer_, BUFFER_SIZE - 1, 0);
    if (bytes_read < 0) {
        Logger::error() << "Socket error after recv";
        return lib::checkSocketError(socket_fd_);
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
            Logger::error()
                << "Error parsing request: " << request_parser_->getStatusCode()
                << " " << request_parser_->getStatusMessage();
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

            // Prepare for response phase using ResponseStreamer
            if (request_handler_->hasCgiRunning()) {
                // CGI: don't prepare response yet, wait for completion
                Logger::debug() << "CGI started, waiting for completion";
            } else if (request_handler_->isStaticFileResponse()) {
                response_streamer_.prepareStaticFile(request_handler_->getStaticFilePath());
                Logger::debug() << "Prepared static file streaming";
            } else {
                response_streamer_.prepareResponse(request_handler_->getResponse());
                Logger::debug() << "Prepared regular response";
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
    // If CGI is running, wait for aux-FD events to complete it
    if (request_handler_ && request_handler_->hasCgiRunning()) {
        return true;
    }

    Logger::debug() << "Writing response, current state: " << response_streamer_.getState();
    
    // Let ResponseStreamer handle all the streaming logic
    ssize_t n = response_streamer_.writeNextChunk(socket_fd_);
    Logger::debug() << "writeNextChunk returned: " << n;
    
    if (n == -1) {
        // Fatal error
        Logger::error() << "Fatal error in response streaming";
        return false;
    }
    
    if (n == -2) {
        // Would block, try again later
        return true;
    }
    
    if (response_streamer_.isComplete()) {
        Logger::debug() << "Response complete, finalizing";
        finalizeResponse();
    } else {
        Logger::debug() << "Response not complete yet, state: " << response_streamer_.getState();
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
    // Static file: poll file for POLLIN when buffer has space
    if (response_streamer_.isStreamingFile() && response_streamer_.wantsFileRead()) {
        pollfd p;
        p.fd = response_streamer_.getFileStream().fileFd();
        p.events = POLLIN;
        p.revents = 0;
        out.push_back(p);
    }
    
    // CGI pipes: poll stdin for POLLOUT, stdout for POLLIN
    if (request_handler_ && request_handler_->hasCgiRunning()) {
        int inFd = request_handler_->getCgiInputPipe();
        int outFd = request_handler_->getCgiOutputPipe();
        
        if (inFd != -1) {
            pollfd p;
            p.fd = inFd;
            p.events = POLLOUT;
            p.revents = 0;
            out.push_back(p);
        }
        
        if (outFd != -1) {
            pollfd p;
            p.fd = outFd;
            p.events = POLLIN;
            p.revents = 0;
            out.push_back(p);
        }
    }
}

bool ClientConnection::HandleAuxEvent(int fd, short revents) {
    // Static file event: read more data into buffer
    if (response_streamer_.isStreamingFile() && 
        fd == response_streamer_.getFileStream().fileFd() && 
        (revents & POLLIN)) {
        response_streamer_.getFileStream().onFileReadable();
    }
    
    // CGI event: handle CGI I/O
    if (request_handler_ && request_handler_->hasCgiRunning()) {
        bool done = request_handler_->handleCgiFdEvent(fd, revents);
        if (done) {
            // CGI complete: prepare response and start streaming
            Logger::debug() << "CGI completed, preparing response";
            response_streamer_.prepareCgiResponse(request_handler_->getResponse());
        }
    }
    
    return true;
}

void ClientConnection::finalizeResponse() {
    Logger::debug() << "Response sent to client " << socket_fd_;
    
    if (request_handler_->shouldCloseConnection()) {
        Logger::debug() << "Connection should be closed";
        Close();
        return;
    }
    
    // Reset for next request
    delete request_handler_;
    request_handler_ = NULL;
    delete request_parser_;
    request_parser_ = NULL;
    
    current_request_.reset();
    state_ = READING_REQUEST;
    request_parser_ = new RequestParser(current_request_, server_config_);
    request_ready_ = false;
    
    // Reset response streamer for next request
    response_streamer_.reset();
    
    UpdateActivity();
}
