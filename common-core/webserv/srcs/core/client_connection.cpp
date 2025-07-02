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

ClientConnection::ClientConnection(int socket_fd)
    : socket_fd_(socket_fd), is_closed_(false) {
    last_activity_ = time(NULL);
    bytes_sent_ = 0;
    request_buffer_ = "";
    response_buffer_ = "";

    // TO DO maybe make it a flag instead
    // or a keep-alive member value with -1 if false
    state_ = KEEP_ALIVE;  // How should state be initialized?
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
    request_buffer_.append(read_buffer_, bytes_read);

    Logger::debug() << "read " << bytes_read << " bytes";
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
    if (state_ == KEEP_ALIVE) {
        return (time(NULL) - last_activity_) > timeout_seconds;
    }
    return true;
}

// State Queries

bool ClientConnection::NeedsToRead() const {
    return state_ == READING_REQUEST || state_ == KEEP_ALIVE;
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
