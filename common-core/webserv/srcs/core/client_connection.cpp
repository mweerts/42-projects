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

#include "../http/RequestParser.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Logger.hpp"
#include "RequestHandler.hpp"
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
      is_closed_(false) {
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

        case CLOSING: {
            Logger::debug() << "Closing connection...";
            return false;
        }
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
            state_ = WRITING_RESPONSE;

            request_handler_ =
                new RequestHandler(current_request_, server_config_);
            request_handler_->handleRequest();

            return true;
        }
        default:
            Logger::error() << "Unknown parse result: " << parse_status;
            return false;
    }

    return true;
}

bool ClientConnection::HandleWrite() {
    request_handler_->sendResponse(socket_fd_);  // should i check for errors?

    if (request_handler_->shouldCloseConnection()) {
        state_ = CLOSING;
        return true;
    }

    // Resetting to handle the next request
    delete request_handler_;
    request_handler_ = NULL;
    delete request_parser_;
    request_parser_ = NULL;

    current_request_.reset();

    state_ = READING_REQUEST;
    request_parser_ = new RequestParser(current_request_, server_config_);
    request_ready_ = false;
    
	UpdateActivity();
    return true;
}

void ClientConnection::Close() {
    if (is_closed_) {
        return;
    }

    if (close(socket_fd_) < 0) {
        Logger::error() << "Failed to close socket fd=" << socket_fd_ << ": "
                        << strerror(errno);
    }

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
    return state_ == CLOSING || state_ == ERROR;
}

int ClientConnection::GetSocketFd() const {
    return socket_fd_;
}

ClientConnection::State ClientConnection::GetState() const {
    return state_;
}
