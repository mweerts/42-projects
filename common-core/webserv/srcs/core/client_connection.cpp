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
#include "../http/utils.hpp"
#include "Logger.hpp"
#include "http_utils.hpp"
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
      request_ready_(false),
      response_streamer_() {
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

void ClientConnection::Close() {
    if (is_closed_)
        return;

    if (close(socket_fd_) < 0) {
        Logger::error() << "Failed to close socket: " << strerror(errno);
    }

    if (request_parser_) {
        request_parser_->cleanup();
    }

    is_closed_ = true;
    socket_fd_ = -1;
}

bool ClientConnection::HandleEvent(short revents) {
    UpdateActivity();

    if (revents & (POLLHUP | POLLERR | POLLNVAL)) {
        Logger::debug() << "Socket error detected: revents=" << revents;
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

static void prepareResponse(RequestHandler*   request_handler,
                            ResponseStreamer& response_streamer) {
    if (!request_handler)
        return;

    HttpResponse& response = request_handler->getResponse();
    if (request_handler->isStaticFileResponse()) {
        response_streamer.prepareStaticFile(
            response, request_handler->getStaticFilePath());
    } else {
        response_streamer.prepareResponse(response);
    }
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
            request_ready_ = true;
            request_handler_ =
                new RequestHandler(current_request_, server_config_);

            StatusCode err_code = request_parser_->getStatusCode();
            Logger::error() << "Invalid request: " << GetHttpStatusText(err_code);
            request_handler_->getResponse().CreateErrorPage(err_code);
            request_handler_->getResponse().setConnection("close");
            prepareResponse(request_handler_, response_streamer_);
            state_ = WRITING_RESPONSE;
            return true;
        }
        case RequestParser::NEED_MORE_DATA: {
            return true;
        }
        case RequestParser::COMPLETE: {
            Logger::debug() << "======== Request parsing complete ========";

            request_ready_ = true;
            request_handler_ =
                new RequestHandler(current_request_, server_config_);
            request_handler_->handleRequest();

            if (!request_handler_->hasCgiRunning()) {
                prepareResponse(request_handler_, response_streamer_);
            }

            state_ = WRITING_RESPONSE;
            return true;
        }
        default:
            Logger::error() << "Unknown parse result: " << parse_status;
            request_handler_->getResponse().CreateErrorPage(
                HTTP_INTERNAL_SERVER_ERROR);
				request_handler_->getResponse().setConnection("close");
            prepareResponse(request_handler_, response_streamer_);
            state_ = WRITING_RESPONSE;
            return false;
    }

    return true;
}

bool ClientConnection::HandleWrite() {
    if (request_handler_ && request_handler_->hasCgiRunning()) {
        return true;
    }

    if (request_handler_ && request_handler_->hasUploadInProgress()) {
        bool finished = request_handler_->processUploadChunk();
        if (finished) {
            HttpResponse& response = request_handler_->getResponse();
            if (request_handler_->uploadSucceeded()) {
                response.setStatusCode(HTTP_CREATED);
            } else {
                response.setStatusCode(HTTP_BAD_REQUEST);
                response.setContent(request_handler_->uploadErrorMessage());
                response.CreateErrorPage();
            }
            prepareResponse(request_handler_, response_streamer_);
        }
        return true;
    }

    ssize_t n = response_streamer_.writeNextChunk(socket_fd_);
    if (n == -1) {
        return false;
    } else if (n == -2) {
        return true;  // EAGAIN or EWOULDBLOCK: try again later
    }

    if (response_streamer_.isComplete()) {
        return finalizeResponse();
    }
    return true;
}

bool ClientConnection::finalizeResponse() {
    Logger::debug() << "Response sent to client " << socket_fd_;

    if (request_handler_->shouldCloseConnection()) {
        Logger::debug() << "Connection should be closed";
        Close();
        return false;
    }

    request_parser_->reset(current_request_);
    delete request_handler_;
    request_handler_ = NULL;

    current_request_.reset();
    response_streamer_.reset();
    state_ = READING_REQUEST;
    request_ready_ = false;
    return true;
}

// ============ OTHER FD EVENTS HANDLING ============ //

std::vector<pollfd> ClientConnection::GetAuxPollFds() const {
    std::vector<pollfd> out;
    // Static file: poll file for POLLIN when buffer has space
    if (response_streamer_.isStreamingFile() &&
        response_streamer_.wantsFileRead()) {
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
    return out;
}

void ClientConnection::HandleAuxEvent(int fd, short revents) {
    // FILE event: read more data into buffer
    if (response_streamer_.isStreamingFile() &&
        (fd == response_streamer_.getFileStream().fileFd()) &&
        (revents & POLLIN)) {
        (void)response_streamer_.getFileStream().onFileReadable();
    }

    // CGI event: handle CGI I/O
    if (request_handler_ && request_handler_->hasCgiRunning()) {
        bool done = request_handler_->handleCgiFdEvent(fd, revents);
        if (done) {
            Logger::debug() << "CGI completed, preparing response";
            response_streamer_.prepareCgiResponse(
                request_handler_->getResponse());
        }
    }
}

// ========== State Queries ========== //

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

// ========== Helpers ========== //

void ClientConnection::UpdateActivity() {
    last_activity_ = time(NULL);
}

bool ClientConnection::IsTimedOut(int timeout_seconds) const {
    if (current_request_.shouldKeepAlive()) {
        return (time(NULL) - last_activity_) > timeout_seconds;
    }
    return false;
}
