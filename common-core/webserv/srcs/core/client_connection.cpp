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

// TODO: 
// TODO: decouple file management from the client connection if possible
// TODO: delete the request file after the request is processed

#include "client_connection.hpp"

#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Logger.hpp"
#include "RequestHandler.hpp"
#include "lib/file_utils.hpp"
#include "lib/utils.hpp"

size_t ClientConnection::request_counter_ = 0;

ClientConnection::ClientConnection(int                 socket_fd,
                                   const ServerConfig& serverConfig)
    : socket_fd_(socket_fd),
      server_config_(serverConfig),
      last_activity_(time(0)),
      request_ready_(false),
      request_file_path_(""),
      request_size_(0),
      chunked_complete_(false),
      bytes_sent_(0),
      response_complete_(false),
      state_(READING_REQUEST),
      keep_alive_(true),
      is_closed_(false) {
    UpdateActivity();
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
                return ReadRequest();
            }
            break;

            // case READING_BODY:
            //     if (revents & POLLIN) {
            //         return HandleBodyRead();
            //     }
            //     break;

            // case READING_CHUNKED:
            //     if (revents & POLLIN) {
            //         return HandleChunkedRead();
            //     }
            //     break;

            // case PROCESSING_REQUEST: return ProcessRequest();

        case WRITING_RESPONSE:
            if (revents & POLLOUT) {  // to implement
                // return HandleWrite();
                return true;
            }
            break;

            // case ERROR: return HandleError();
        case CLOSING: return false;
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

bool ClientConnection::ReadRequest() {
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
    if (!saveRequestToFile(read_buffer_)) {
        return false;
    }

	// i should have a way to check if i still need to read more 
	// and return the correct state

	// even better i delegate saving the request to request handler
	// an it returns a state that i can use to know if i need to read more
	
    try {
        request_buffer_.append(read_buffer_, bytes_read);
    } catch (std::exception& e) {
        Logger::error() << e.what();
        return false;
    }

    // Logger::debug() << "read " << bytes_read << " bytes";
    // Logger::debug() << read_buffer_;

    RequestHandler handler = RequestHandler(server_config_);
	handler.handleRequest(request_buffer_);
    handler.sendResponse(socket_fd_);

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
    // response_buffer_.clear();
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

// FILE MANAGEMENT

// should i have a version of this that trunc instead of append?
bool ClientConnection::saveRequestToFile(const char* buffer) {
    if (!buffer) {
        return false;
    }

    if (request_file_path_.empty()) {  // means we are on the first read
        try {
            request_file_path_ = getRequestFilePath();
        } catch (std::exception& e) {
            Logger::critical() << e.what();
            return false;
        }
        if (request_file_path_.empty()) {
            Logger::error() << "Failed to create temporary request file";
            return false;
        }
        request_file_.open(request_file_path_.c_str(),
                           std::ios::out | std::ios::trunc | std::ios::binary);
        if (!request_file_.is_open()) {
            Logger::error() << "Failed to open temporary request file";
            return false;
        }
    }

    request_file_ << std::string(buffer);
    request_file_.flush();  // force write to file
    if (request_file_.fail()) {
        Logger::error() << "Failed to write to file: " << request_file_path_;
        return false;
    }
    return true;
}

std::string ClientConnection::getRequestFilePath() {
    if (request_file_path_.empty()) {
        std::string dir = server_config_.getRoot() + "/tmp";

        if (!lib::pathExist(dir)) {
            Logger::debug() << "Creating tmp directory for request: " << dir;
            mkdir(dir.c_str(), 0755);
        } else if (!lib::isDirectory(dir)) {
            throw std::runtime_error(
                "Failed to create tmp directory for request");
        } else if (!lib::isWritable(dir)) {
            throw std::runtime_error(
                "Failed to access tmp directory for request");
        }

        request_file_path_ = dir + "/" + lib::to_string(time(NULL)) +
                             lib::to_string(request_counter_++) + "-" +
                             lib::to_string(socket_fd_);

        Logger::debug() << "Created temporary request file: "
                        << request_file_path_;
    }
    return request_file_path_;
}

// State Queries

bool ClientConnection::NeedsToRead() const {
    return state_ == READING_REQUEST || state_ == READING_BODY ||
           state_ == READING_CHUNKED;
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
