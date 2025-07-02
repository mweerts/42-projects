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
#include <ctime>
#include <cerrno>
#include <cstring>
#include "Logger.hpp"
#include <unistd.h>

ClientConnection::ClientConnection(int socket_fd) : socket_fd_(socket_fd), is_closed_(false) {
    last_activity_ = time(NULL);
    bytes_sent_ = 0;
    request_buffer_ = "";
    response_buffer_ = "";
    
    // TO DO maybe make it a flag instead
    // or a keep-alive member value with -1 if false
    state_ = KEEP_ALIVE; // How should state be initialized?
}

void ClientConnection::Close() {
    if (is_closed_) {
        return;
    }
    
    if (close(socket_fd_) < 0) {
        Logger::error() << "Failed to close socket fd=" << socket_fd_ 
                       << ": " << strerror(errno);
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