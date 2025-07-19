/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection_manager.hpp                              :+:      :+:    :+: */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <lucas.lebugle@student.s19.be>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/XX XX:XX:XX by llebugle          #+#    #+#             */
/*   Updated: 2025/01/XX XX:XX:XX by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

#include <sys/poll.h>

#include <map>
#include <vector>

#include "client_connection.hpp"
#include "tcp_server.hpp"

#if defined(__APPLE__)
	typedef volatile sig_atomic_t shutdown_flag_t;
#else
	typedef volatile __sig_atomic_t shutdown_flag_t;
#endif

class ConnectionManager {
   public:
    ConnectionManager();
    ~ConnectionManager();

    void Run();
    void Stop();

    void AddServer(TcpServer* server);

    size_t GetActiveClientCount() const;
    bool   IsRunning() const;

    void SetShutdownFlag(shutdown_flag_t* shutdown_flag);

   private:
    std::vector<pollfd>              poll_fds_;
    std::map<int, TcpServer*>        servers_;
    std::map<int, ClientConnection*> clients_;
    bool                             running_;

    shutdown_flag_t* shutdown_flag_;

    typedef std::map<int, ClientConnection*>::iterator ClientIterator;
    typedef std::map<int, ClientConnection*>::const_iterator
                                                      ClientConstIterator;
    typedef std::map<int, TcpServer*>::const_iterator ServerConstIterator;

   private:
    void SetupPolling();
    void HandleNewConnection(int fd);
    void HandleClientEvent(int fd, short events);
    void CleanupTimedOutClients();
    void RemoveClient(int client_fd);

    bool                IsServerSocket(int fd) const;
    const ServerConfig& GetServerConfig(int fd) const;
};

#endif
