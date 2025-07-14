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

#include "Logger.hpp"
#include "server_config.hpp"
#include "RequestHandler.hpp"

ClientConnection::ClientConnection(int socket_fd)
    : socket_fd_(socket_fd),
      state_(READING_REQUEST),
      keep_alive_(true),
      last_activity_(time(0)),
      client_max_header_size_(ServerConfig::DEFAULT_MAX_HEADER_SIZE),
      client_max_request_line_(ServerConfig::DEFAULT_MAX_REQUEST_LINE),
      client_max_body_size_(ServerConfig::DEFAULT_MAX_BODY_SIZE),
      bytes_sent_(0),
      is_closed_(false) {
    UpdateActivity();

    request_buffer_.reserve(client_max_header_size_);
}

ClientConnection::ClientConnection(int socket_fd, const ServerConfig& config)
    : socket_fd_(socket_fd),
      state_(READING_REQUEST),
      keep_alive_(true),
      last_activity_(time(0)),
      client_max_header_size_(config.client_max_header_size),
      client_max_request_line_(config.client_max_request_line),
      client_max_body_size_(config.client_max_body_size),
      bytes_sent_(0),
      is_closed_(false) {
    UpdateActivity();

    request_buffer_.reserve(client_max_header_size_);
}

bool ClientConnection::HandleEvent(short revents) {
    UpdateActivity();

    if (revents & (POLLHUP | POLLERR | POLLNVAL)) {
        Logger::debug() << "Client " << socket_fd_ << " disconnected";
        return false;
    }

    if (revents & POLLIN) {
        if (!HandleRead()) {
            return false;
        }
    }

    // if (revents & POLLOUT) {
    //     if (!HandleWrite()) {
    //         return false;
    //     }
    // }

    if (ShouldClose()) {
        return false;
    }

    return true;
}

bool ClientConnection::HandleRead() {
    // TODO use config limits for reading (i.e max_client_size)

    ssize_t bytes_read = recv(socket_fd_, read_buffer_, BUFFER_SIZE - 1, 0);
    if (bytes_read < 0) {
        int       err = 0;
        socklen_t len = sizeof(err);
        getsockopt(socket_fd_, SOL_SOCKET, SO_ERROR, &err, &len);

        if (!err) {
            return true;
        }

        Logger::error() << "Socket error on client " << socket_fd_;
        return false;
    }

    if (bytes_read == 0) {
        Logger::debug() << "Client " << socket_fd_ << " closed connection";
        return false;
    }

    read_buffer_[bytes_read] = '\0';
    try {
        request_buffer_.append(read_buffer_, bytes_read);
    } catch (std::exception& e) {
        Logger::error() << e.what();
        return false;
    }

    Logger::debug() << "read " << bytes_read << " bytes";
    Logger::debug() << read_buffer_;
    
    RequestHandler handler = RequestHandler();
    handler.handleRequest(read_buffer_);
    handler.sendResponse(socket_fd_);

    // Stopped here
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

    request_buffer_.clear();
    response_buffer_.clear();
    is_closed_ = true;
    socket_fd_ = -1;
}

void ClientConnection::UpdateActivity() {
    last_activity_ = time(NULL);
}

bool ClientConnection::IsTimedOut(int timeout_seconds) const {
    if (keep_alive_) {
        return (time(NULL) - last_activity_) > timeout_seconds;
    }
    return true;
}

// State Queries

bool ClientConnection::NeedsToRead() const {
    return state_ == READING_REQUEST;
}

bool ClientConnection::NeedsToWrite() const {
    return state_ == WRITING_RESPONSE;
}

bool ClientConnection::ShouldClose() const {
    return state_ == CLOSING;
}

int ClientConnection::GetSocket() const {
    return socket_fd_;
}

ClientConnection::State ClientConnection::GetState() const {
    return state_;
}
