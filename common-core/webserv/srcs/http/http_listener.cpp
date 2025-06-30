/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_listeners.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 22:15:42 by llebugle          #+#    #+#             */
/*   Updated: 2025/06/27 22:15:43 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http_listener.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <iostream>

#include "Logger.hpp"
#include "lib/socket_guard.hpp"
#include "lib/utils.hpp"

bool HttpListener::Initialize() {
    listen_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd_ < 0) {
        Logger::error() << "Failed to create socket for " << config_.name;
        return false;
    }

    lib::SocketGuard guard(listen_fd_);

    int opt = 1;  // no wait between restart (convenience), maybe remove later
    if (setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) <
        0) {
        Logger::error() << "Setsockopt failed for " << config_.name;
        return false;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;  // IPV4
    address.sin_port = htons(config_.port);
    address.sin_addr.s_addr = getAddress();
    if (address.sin_addr.s_addr == INADDR_NONE) {
        Logger::error() << "Invalid IP: " << config_.host;
        return false;
    }

    if (bind(listen_fd_, (sockaddr*)&address, sizeof(address)) < 0) {
        Logger::error() << "Bind failed for " << config_.name << " on port "
                        << config_.port << ": " << strerror(errno);
        return false;
    }

    if (listen(listen_fd_, 10) < 0) {
        Logger::error() << "Listen failed for " << config_.name;
        return false;
    }

    guard.release();

    Logger::info() << "Server '" << config_.name << "' listening on "
                   << config_.host << ":" << config_.port;

    return true;
}

in_addr_t HttpListener::getAddress() {
    in_addr_t host_addr;
    if (config_.host.empty()) {
        return INADDR_ANY;
    } else if (config_.host == "localhost") {
        return inet_addr("127.0.0.1");
    } else {
        host_addr = inet_addr(config_.host.c_str());
    }
    return host_addr;
}

int HttpListener::GetListenSocket() const {
    return listen_fd_;
}

HttpListener::HttpListener(const HttpListener& other) {
    (void)other;
};
HttpListener& HttpListener::operator=(const HttpListener& other) {
    (void)other;
    return *this;
}

// OLD VERSION TO TEST
// void HttpListener::AcceptAndHandle() {
//     // Accept one incoming connection
//     int client_fd = accept(listen_fd_, NULL, NULL);
//     if (client_fd < 0) {
//         // Don't spam errors - accept can fail for various reasons
//         return;
//     }

//     std::cout << "[" << config_.name << "] New connection accepted"
//               << std::endl;

//     // Handle the request immediately (blocking)
//     HandleRequest(client_fd);
// }

// void HttpListener::HandleRequest(int client_fd) {
//     // TODO
//     // read and parse request
//     std::string response = BuildHttpResponse();

//     ssize_t bytes_sent =
//         send(client_fd, response.c_str(), response.length(), 0);
//     if (bytes_sent < 0) {
//         std::cerr << "[" << config_.name << "] Failed to send response"
//                   << std::endl;
//     } else {
//         std::cout << "[" << config_.name << "] Response sent (" << bytes_sent
//                   << " bytes)" << std::endl;
//     }
// }

// std::string HttpListener::BuildHttpResponse() {
//     // TODO Implement real Response builder
//     // probably a class with the different methods
//     std::string body = "Hello from server: " + config_.name + " (port " +
//                        lib::to_string(config_.port) + ")";

//     std::string response = "HTTP/1.1 200 OK\r\n";
//     // response += "Connection: close\r\n";  // Explicitly close connection
//     response +=
//         "Content-Length: " + lib::to_string(static_cast<int>(body.length()))
//         +
//         "\r\n";
//     response += "\r\n";
//     response += body;

//     return response;
// }
