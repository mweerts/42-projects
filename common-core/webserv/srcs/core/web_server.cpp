/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   web_server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <lucas.lebugle@student.s19.be>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 22:04:00 by llebugle          #+#    #+#             */
/*   Updated: 2025/07/19 02:01:20 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "web_server.hpp"

#include <signal.h>

#include <iostream>
#include <vector>

#include "Logger.hpp"

volatile sig_atomic_t g_shutdown_requested = 0;

void signal_handler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        g_shutdown_requested = 1;
    }
}

WebServer::WebServer(const GlobalConfig& config)
    : server_configs_(config.getServers()), connection_manager_(NULL) {}

WebServer::WebServer(const std::vector<ServerConfig>& server_configs)
    : server_configs_(server_configs), connection_manager_(NULL) {}

WebServer::~WebServer() {
    CleanupServers();
	Logger::cleanup();
    if (connection_manager_) {
        delete connection_manager_;
    }
}

bool WebServer::Start() {
    if (!InitializeServers()) {
        Logger::critical() << "Failed to initialize servers.";
        return false;
    }

    connection_manager_ = new ConnectionManager();
    connection_manager_->SetShutdownFlag(&g_shutdown_requested);

    for (std::vector<TcpServer*>::iterator it = tcp_servers_.begin();
         it != tcp_servers_.end(); ++it) {
        connection_manager_->AddServer(*it);
    }

    if (tcp_servers_.empty()) {
        Logger::critical() << "No servers could be initialized.";
        Stop();
        return false;
    }

    Logger::debug() << "WebServer started with " << tcp_servers_.size()
                    << " servers";
    return true;
}

void WebServer::Run() {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    Logger::info() << "WebServer is running...";

	if (!server_configs_.empty()) {
		std::string path = server_configs_[0].getRoot() + "/webserv.log";
		Logger::setLogFile(path);
	} 

    if (connection_manager_) {
        connection_manager_->Run();
    }

    Stop();
}

void WebServer::Stop() {
    Logger::info() << "... Shutting down server ...";

    if (connection_manager_) {
        connection_manager_->Stop();
    }

    Logger::info() << "WebServer stopped gracefully.";
}

void WebServer::Reset() {
    if (connection_manager_) {
        connection_manager_->Stop();
    }

    CleanupServers();

    Logger::debug() << "Server reset...";
}

bool WebServer::InitializeServers() {
    for (std::vector<ServerConfig>::const_iterator it = server_configs_.begin();
         it != server_configs_.end(); ++it) {
        TcpServer* server = new TcpServer(*it);
        if (!server->Initialize()) {
            Logger::warning()
                << "Failed to initialize server: " << it->getName();
            delete server;
            continue;
        }
        tcp_servers_.push_back(server);
    }

    if (tcp_servers_.size() < server_configs_.size()) {
        Logger::warning()
            << "Partial startup: some servers failed to initialize";
    }

    return !tcp_servers_.empty();
}

void WebServer::CleanupServers() {
    for (std::vector<TcpServer*>::iterator it = tcp_servers_.begin();
         it != tcp_servers_.end(); ++it) {
        delete *it;
    }
    tcp_servers_.clear();
}
