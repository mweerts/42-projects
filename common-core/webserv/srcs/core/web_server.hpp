/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   web_server.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <lucas.lebugle@student.s19.be>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 20:57:20 by llebugle          #+#    #+#             */
/*   Updated: 2025/07/19 01:47:06 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEB_SERVER_HPP
#define WEB_SERVER_HPP

#include <vector>

#include "../parsing/include/GlobalConfig.hpp"
#include "connection_manager.hpp"
#include "tcp_server.hpp"

class WebServer {
   public:
    explicit WebServer(const GlobalConfig& config);
    explicit WebServer(const std::vector<ServerConfig>& server_configs);
    ~WebServer();

    bool Start();
    void Run();
    void Stop();
    void Reset();

   private:
    std::vector<ServerConfig> server_configs_;
    std::vector<TcpServer*>   tcp_servers_;
    ConnectionManager*        connection_manager_;

    bool InitializeServers();
    void CleanupServers();
};

#endif
