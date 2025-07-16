/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   web_server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 22:04:00 by llebugle          #+#    #+#             */
/*   Updated: 2025/06/27 22:04:01 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "web_server.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/poll.h>
#include <sys/socket.h>

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <vector>

#include "../http/http_server.hpp"
#include "Logger.hpp"
#include "client_connection.hpp"
#include "lib/socket_guard.hpp"

// TODO: should we keep this for graceful exit?
volatile sig_atomic_t g_shutdown_requested = 0;

void signal_handler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        g_shutdown_requested = 1;
    }
}

void WebServer::Run() {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    running_ = true;
    
    while (running_ && !g_shutdown_requested) {
        SetupPolling();
        int ready = poll(poll_fds_.data(), poll_fds_.size(), 100);

        if (ready < 0 && !g_shutdown_requested) {
            if (errno == EINTR) {
                Logger::debug() << "Poll interrupted by signal, retrying";
                continue;
            } else {
                Logger::critical() << "Poll error: " << strerror(errno);
                break;
            }
        }

        if (ready == 0) {
            CleanupTimedOutClients();
            continue;
        }

        for (size_t i = 0; i < poll_fds_.size() && ready > 0; ++i) {
            if (poll_fds_[i].revents == 0) {
                continue;
            }

            --ready;
            int fd = poll_fds_[i].fd;
            if (IsListeningSocket(fd)) {
                HandleNewConnection(fd);
            } else {
                ClientConnection* client = active_clients_[fd];
                if (!client->HandleEvent(poll_fds_[i].revents)) {
                    RemoveClient(fd);
                }
            }
        }
    }
    Stop();
}

void WebServer::SetupPolling() {
    poll_fds_.clear();

    for (ServerConstIterator it = http_servers_.begin();
         it != http_servers_.end(); ++it) {
        pollfd pfd;
        pfd.fd = it->second->GetListenSocket();
        pfd.events = POLLIN;
        pfd.revents = 0;
        poll_fds_.push_back(pfd);
    }

    for (std::map<int, ClientConnection*>::const_iterator it =
             active_clients_.begin();
         it != active_clients_.end(); ++it) {
        ClientConnection* client = it->second;

        if (client->ShouldClose()) {
            continue;
        }

        pollfd pfd;
        pfd.fd = it->first;
        pfd.events = 0;
        pfd.revents = 0;

        if (client->NeedsToRead()) {
            pfd.events |= POLLIN;
        }
        if (client->NeedsToWrite()) {
            pfd.events |= POLLOUT;
        }
        if (pfd.events != 0) {
            poll_fds_.push_back(pfd);
        }
    }
}

void WebServer::HandleNewConnection(int listening_fd) {
    struct sockaddr_in client_addr;

    socklen_t clen = sizeof(client_addr);
    int client_fd = accept(listening_fd, (struct sockaddr*)&client_addr, &clen);
    if (client_fd < 0) {
        if (errno != EWOULDBLOCK && errno != EAGAIN) {  // skips common errors
            Logger::warning()
                << "Failed to accept connection: " << strerror(errno);
        }
        return;
    }

    lib::SocketGuard guard(client_fd);

#ifdef __APPLE__  // set non-blocking I/O on mac
    int flags = fcntl(client_fd, F_GETFL, 0);
    if (flags < 0) {
        Logger::error() << "Failed to get socket flags for fd=" << client_fd;
        return;
    }

    if (fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        Logger::error() << "Failed to set non-blocking for fd=" << client_fd;
        return;
    }
#endif


    const ServerConfig& Server_config = GetServerConfig(listening_fd);
    active_clients_[client_fd] = new ClientConnection(client_fd, Server_config);
    guard.release();

    // TEMPORARY: only for Logging
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);

    Logger::info() << "New client connected: fd=" << client_fd << " from "
                   << client_ip << ":" << ntohs(client_addr.sin_port)
                   << ". Active clients: " << active_clients_.size();
    // END TEMPORARY
}

const ServerConfig& WebServer::GetServerConfig(int fd) const {
    ServerConstIterator it = http_servers_.find(fd);
    if (it != http_servers_.end()) {
        return it->second->GetConfig();
    }

    Logger::critical() << "No server config found for fd: " << fd;
    return config_.getServers()[0];
}

void WebServer::CleanupTimedOutClients() {
    std::vector<int> clients_to_close;

    for (ActiveClientConstIterator it = active_clients_.begin();
         it != active_clients_.end(); ++it) {
        int               client_fd = it->first;
        ClientConnection* client = it->second;

        if (client->IsTimedOut()) {
            Logger::debug() << "Client fd=" << client_fd << " timed out";
            clients_to_close.push_back(client_fd);
        }
    }

    for (std::vector<int>::iterator it = clients_to_close.begin();
         it != clients_to_close.end(); ++it) {
        RemoveClient(*it);
    }
}

void WebServer::RemoveClient(int client_fd) {
    ActiveClientIterator it = active_clients_.find(client_fd);
    if (it == active_clients_.end()) {
        Logger::warning() << "Attempted to remove non-existent client fd";
        return;
    }

    ClientConnection* client = it->second;

    client->Close();

    delete client;
    active_clients_.erase(it);

    Logger::debug() << "Removed client fd=" << client_fd
                    << ". Active clients: " << active_clients_.size();
}

bool WebServer::IsListeningSocket(int fd) const {
    return http_servers_.find(fd) != http_servers_.end();
}

bool WebServer::Start() {
    server_configs_ = config_.getServers();
    for (std::vector<ServerConfig>::const_iterator it = server_configs_.begin();
         it != server_configs_.end(); ++it) {
        http::Server* Server = new http::Server(*it);
        if (!Server->Initialize()) {
            delete Server;
            continue;
        }
        http_servers_[Server->GetListenSocket()] = Server;
    }
    
    if (http_servers_.empty()) {
        Logger::critical() << "No servers could be initialized.";
        return false;
    }

    if (http_servers_.size() < server_configs_.size()) {
        Logger::warning()
            << "Partial start up: some servers failed to initialize";
    }

    SetupPolling();

    Logger::debug() << "WebServer started with " << http_servers_.size()
                    << " servers";
    return true;
}

void WebServer::Stop() {
    running_ = false;

    std::cout << "\n";
    Logger::info() << "... Shutting down server ...";

    std::vector<int> clients_to_close;
    for (ActiveClientConstIterator it = active_clients_.begin();
         it != active_clients_.end(); ++it) {
        clients_to_close.push_back(it->first);
    }

    for (std::vector<int>::iterator it = clients_to_close.begin();
         it != clients_to_close.end(); ++it) {
        RemoveClient(*it);
    }

    Logger::info() << "WebServer stopped gracefully. Closed all connections";
}

void WebServer::Reset() {
    running_ = false;

    std::vector<int> clients_to_close;
    for (ActiveClientConstIterator it = active_clients_.begin();
         it != active_clients_.end(); ++it) {
        clients_to_close.push_back(it->first);
    }

    for (std::vector<int>::iterator it = clients_to_close.begin();
         it != clients_to_close.end(); ++it) {
        RemoveClient(*it);
    }

    Logger::debug() << "Server reset...";
}
