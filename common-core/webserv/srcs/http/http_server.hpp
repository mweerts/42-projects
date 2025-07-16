/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_server.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 21:48:09 by llebugle          #+#    #+#             */
/*   Updated: 2025/06/27 21:48:11 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <netinet/in.h>
#include <unistd.h>

#include <string>

#include "../parsing/include/GlobalConfig.hpp"

namespace http {
class Server {
   public:
    explicit Server(const ServerConfig& config)
        : config_(config), listen_fd_(-1) {}
    Server(const Server& other) : config_(other.config_) {};
    Server& operator=(const Server& other) {
        if (this != &other) {
            this->listen_fd_ = other.listen_fd_;
        }
        return *this;
    };
    ~Server() {
        if (listen_fd_ >= 0) {
            close(listen_fd_);
        }
    }

    bool Initialize();  // Create socket, bind and listen
    int  GetListenSocket() const;

    const ServerConfig& GetConfig() const;

   private:
    const ServerConfig& config_;
    int                 listen_fd_;

   private:
    void HandleRequest(int client_fd);
    bool BindAddress();
};
}  // namespace http

#endif
