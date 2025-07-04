/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 22:15:42 by llebugle          #+#    #+#             */
/*   Updated: 2025/06/27 22:15:43 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http_server.hpp"

#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>

#include <cerrno>
#include <cstring>
#include <string>

#include "Logger.hpp"
#include "lib/socket_guard.hpp"
#include "lib/utils.hpp"
#include "server_config.hpp"

namespace http {
bool Server::Initialize() {
#ifdef __linux__
    listen_fd_ = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
#elif defined(__APPLE__) || defined(__unix__)
    listen_fd_ = socket(AF_INET, SOCK_STREAM, 0);

    int flags = fcntl(listen_fd_, F_GETFL, 0);
    if (flags != -1) {
        fcntl(listen_fd_, F_SETFL, flags | O_NONBLOCK);
    }
#else
#error "Unsupported platform"
#endif

    if (listen_fd_ < 0) {
        Logger::error() << "Failed to create socket for " << config_.name;
        return false;
    }

    lib::SocketGuard guard(listen_fd_);

    int opt = 1;
    if (setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) <
        0) {
        Logger::error() << "Setsockopt failed for " << config_.name;
        return false;
    }

    if (!BindAddress()) {
        return false;
    }

    const int kBacklog = 128;
    if (listen(listen_fd_, kBacklog) < 0) {
        Logger::error() << "Listen failed for " << config_.name;
        return false;
    }

    guard.release();

    Logger::info() << "Server '" << config_.name << "' listening on "
                   << (config_.host.empty() ? "0.0.0.0" : config_.host) << ":"
                   << config_.port;

    return true;
}

bool Server::BindAddress() {
    struct addrinfo  hints;
    struct addrinfo* result;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_flags = AI_PASSIVE;      // binds to all interfaces if !host

    std::string port = lib::to_string(config_.port);
    const char* host = config_.host.empty() ? NULL : config_.host.c_str();

    int status = getaddrinfo(host, port.c_str(), &hints, &result);
    if (status != 0) {
        Logger::error() << "getaddrinfo failed: " << config_.name << " ("
                        << (host ? host : "0.0.0.0") << ":" << config_.port
                        << "): " << gai_strerror(status);
        return false;
    }

    bool bind_success = false;
    for (struct addrinfo* addr = result; addr != NULL; addr = addr->ai_next) {
        if (bind(listen_fd_, addr->ai_addr, addr->ai_addrlen) == 0) {
            bind_success = true;
            break;
        }
    }

    freeaddrinfo(result);
    if (!bind_success) {
        Logger::error() << "Bind failed for " << config_.name << " on port "
                        << config_.port << ": " << strerror(errno);
        return false;
    }
    return true;
}

const ServerConfig& Server::GetConfig() const {
    return config_;
}

int Server::GetListenSocket() const {
    return listen_fd_;
}
}  // namespace http
