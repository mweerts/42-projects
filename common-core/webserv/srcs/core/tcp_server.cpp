/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcp_server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <lucas.lebugle@student.s19.be>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 00:00:00 by llebugle          #+#    #+#             */
/*   Updated: 2025/07/18 22:01:50 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tcp_server.hpp"

#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>

#include <cerrno>
#include <cstring>
#include <string>

#include "Logger.hpp"
#include "lib/socket_guard.hpp"
#include "lib/utils.hpp"
#include "../parsing/GlobalConfig.hpp"

TcpServer::TcpServer(const ServerConfig& config)
    : config_(config), listen_fd_(-1) {}

TcpServer::TcpServer(const TcpServer& other)
    : config_(other.config_), listen_fd_(-1) {}

TcpServer& TcpServer::operator=(const TcpServer& other) {
    if (this != &other) {
        this->listen_fd_ = other.listen_fd_;
    }
    return *this;
}

TcpServer::~TcpServer() {
    if (listen_fd_ >= 0) {
        close(listen_fd_);
    }
}

bool TcpServer::Initialize() {
#ifdef __linux__
    listen_fd_ = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
#elif defined(__APPLE__) || defined(__unix__)
    listen_fd_ = socket(AF_INET, SOCK_STREAM, 0);
#else
#error "Unsupported platform"
#endif

    if (listen_fd_ < 0) {
        Logger::error() << "Failed to create socket for " << config_.getName();
        return false;
    }

    lib::SocketGuard guard(listen_fd_);  // if not released, automatically
                                         // closes the socket on scope exit

    int opt = 1;
    if (setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) <
        0) {
        Logger::error() << "Setsockopt failed for " << config_.getName();
        return false;
    }

    if (!BindAddress()) {
        return false;
    }

    const int kBacklog = 128;  // pending connections queue (128 is standard)
    if (listen(listen_fd_, kBacklog) < 0) {
        Logger::error() << "Listen failed for " << config_.getName();
        return false;
    }

    guard.release();

    Logger::info() << "Server '" << config_.getName() << "' listening on "
                   << config_.getHost() << ":" << config_.getPort();

    return true;
}

bool TcpServer::BindAddress() {
    struct addrinfo  hints;
    struct addrinfo* result;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    std::string port = lib::to_string(config_.getPort());
    std::string host = config_.getHost().empty() ? "" : config_.getHost();

    int status = getaddrinfo(host.c_str(), port.c_str(), &hints, &result);
    if (status != 0) {
        Logger::error() << "getaddrinfo failed: " << config_.getName() << " ("
                        << (!host.empty() ? host : "0.0.0.0") << ":"
                        << config_.getPort() << "): " << gai_strerror(status);
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
        Logger::error() << "Bind failed for " << config_.getName()
                        << " on port " << config_.getPort() << ": "
                        << strerror(errno);
        return false;
    }
    return true;
}

int TcpServer::GetListenSocket() const {
    return listen_fd_;
}

const ServerConfig& TcpServer::GetConfig() const {
    return config_;
}
