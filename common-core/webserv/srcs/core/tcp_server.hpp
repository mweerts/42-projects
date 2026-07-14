/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcp_server.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <lucas.lebugle@student.s19.be>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 22:00:31 by llebugle          #+#    #+#             */
/*   Updated: 2025/07/18 22:06:39 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <netinet/in.h>
#include <unistd.h>

#include <string>

class GlobalConfig;
class ServerConfig;

class TcpServer {
   public:
    explicit TcpServer(const ServerConfig& config);
    TcpServer(const TcpServer& other);
    TcpServer& operator=(const TcpServer& other);
    ~TcpServer();

    bool Initialize();

    int                 GetListenSocket() const;
    const ServerConfig& GetConfig() const;

   private:
    const ServerConfig& config_;
    int                 listen_fd_;

    bool BindAddress();
};

#endif
