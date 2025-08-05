/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection_manager.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <lucas.lebugle@student.s19.be>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/XX XX:XX:XX by llebugle          #+#    #+#             */
/*   Updated: 2025/01/XX XX:XX:XX by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// this is the tcp handler maybe it should be called tcp_manager

#include "connection_manager.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>

#include "Logger.hpp"
#include "lib/socket_guard.hpp"
#include "../parsing/GlobalConfig.hpp"

ConnectionManager::ConnectionManager() : running_(false), shutdown_flag_(NULL) {}

ConnectionManager::~ConnectionManager() {
    for (ClientIterator it = clients_.begin(); it != clients_.end(); ++it) {
        delete it->second;
    }
    clients_.clear();
}

void ConnectionManager::Run() {
    running_ = true;
    
    while (running_ && (shutdown_flag_ == NULL || !*shutdown_flag_)) {
        SetupPolling();
        int ready = poll(poll_fds_.data(), poll_fds_.size(), 100);

        if (ready < 0 && errno != EINTR) {
            Logger::critical() << "Poll error: " << strerror(errno);
            break;
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
            
            if (IsServerSocket(fd)) {
                HandleNewConnection(fd);
            } else {
                HandleClientEvent(fd, poll_fds_[i].revents);
            }
        }
    }
	std::cout << "\n";
}

void ConnectionManager::Stop() {
    running_ = false;
    
    while (!clients_.empty()) {
        RemoveClient(clients_.begin()->first);
    }

    Logger::debug() << "ConnectionManager closed all connections";
}

void ConnectionManager::AddServer(TcpServer* server) {
    int listening_fd = server->GetListenSocket();
    if (server && listening_fd >= 0) {
        servers_[listening_fd] = server;
        Logger::debug() << "Added server with fd=" << listening_fd;
    }
}

void ConnectionManager::SetupPolling() {
    poll_fds_.clear();

    for (ServerConstIterator it = servers_.begin(); it != servers_.end(); ++it) {
        pollfd pfd;
        pfd.fd = it->first;
        pfd.events = POLLIN;
        pfd.revents = 0;
        poll_fds_.push_back(pfd);
    }

    for (ClientConstIterator it = clients_.begin(); it != clients_.end(); ++it) {
        ClientConnection* client = it->second;

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

void ConnectionManager::HandleNewConnection(int listening_fd) {
    struct sockaddr_in client_addr;
    socklen_t clen = sizeof(client_addr);
    int client_fd = accept(listening_fd, (struct sockaddr*)&client_addr, &clen);
    
    if (client_fd < 0) {
        if (errno != EWOULDBLOCK && errno != EAGAIN) {
            Logger::warning() << "Failed to accept connection: " << strerror(errno);
        }
        return;
    }

    lib::SocketGuard guard(client_fd);

#ifdef __APPLE__
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

    const ServerConfig& server_config = GetServerConfig(listening_fd);
    clients_[client_fd] = new ClientConnection(client_fd, server_config);
    guard.release();

	// only for info logging
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);

    Logger::info() << "New client connected: fd=" << client_fd << " from "
                   << client_ip << ":" << ntohs(client_addr.sin_port)
                   << ". Active clients: " << clients_.size();
}

void ConnectionManager::HandleClientEvent(int fd, short events) {
    ClientIterator it = clients_.find(fd);
    if (it == clients_.end()) {
        Logger::warning() << "Event for unknown client fd=" << fd;
        return;
    }

    ClientConnection* client = it->second;
    if (!client->HandleEvent(events)) {
        RemoveClient(fd);
    }
}

void ConnectionManager::RemoveClient(int client_fd) {
    ClientIterator it = clients_.find(client_fd);
    if (it == clients_.end()) {
        Logger::warning() << "Attempted to remove non-existent client fd";
        return;
    }

    ClientConnection* client = it->second;
    client->Close();
    delete client;
    clients_.erase(it);

    Logger::debug() << "Removed client fd=" << client_fd
                    << ". Active clients: " << clients_.size();
}

void ConnectionManager::CleanupTimedOutClients() {
    std::vector<int> clients_to_close;

    for (ClientConstIterator it = clients_.begin(); it != clients_.end(); ++it) {
        int client_fd = it->first;
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

bool ConnectionManager::IsServerSocket(int fd) const {
    return servers_.find(fd) != servers_.end();
}

const ServerConfig& ConnectionManager::GetServerConfig(int fd) const {
    ServerConstIterator it = servers_.find(fd);
    if (it != servers_.end()) {
        return it->second->GetConfig();
    }
    
    Logger::critical() << "No server config found for fd: " << fd;
    static ServerConfig dummy;
    return dummy;
}

size_t ConnectionManager::GetActiveClientCount() const {
    return clients_.size();
}

bool ConnectionManager::IsRunning() const {
    return running_;
} 

void ConnectionManager::SetShutdownFlag(shutdown_flag_t* shutdown_flag) {
    shutdown_flag_ = shutdown_flag;
} 
