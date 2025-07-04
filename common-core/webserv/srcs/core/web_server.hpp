/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 20:57:20 by llebugle          #+#    #+#             */
/*   Updated: 2025/06/27 20:57:21 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEB_SERVER_HPP
#define WEB_SERVER_HPP

#include <sys/poll.h>

#include <map>
#include <vector>

#include "../http/http_server.hpp"
#include "client_connection.hpp"
#include "server_config.hpp"

class WebServer {
   public:
    explicit WebServer(Config& config) : config_(config), running_(false) {};

    ~WebServer() {
        for (ServerIterator it = http_servers_.begin();
             it != http_servers_.end(); ++it) {
            delete *it;
        }

        for (ActiveClientIterator it = active_clients_.begin();
             it != active_clients_.end(); ++it) {
            delete it->second;
        }
    }

    bool Start();
    void Run();
    void Stop();
    void Reset();

   private:
    std::vector<http::Server*> http_servers_;
    std::vector<pollfd>        poll_fds_;
    Config                     config_;
    bool                       running_;

    std::map<int, ClientConnection*> active_clients_;

    typedef std::map<int, ClientConnection*>::iterator ActiveClientIterator;
    typedef std::map<int, ClientConnection*>::const_iterator
                                                 ActiveClientConstIterator;
    typedef std::vector<http::Server*>::iterator ServerIterator;
    typedef std::vector<http::Server*>::const_iterator ServerConstIterator;

   private:
    bool Initializeservers();
    void SetupPolling();

    bool IsListeningSocket(int fd) const;
    void HandleNewConnection(int listening_fd);
    void RemoveClient(int client_fd);
    void CleanupTimedOutClients();

    const ServerConfig& GetServerConfig(int fd) const;

    // void HandleClientRequest(int client_fd);
    // void HandleClientResponse(int client_fd);
    //
};

#endif
