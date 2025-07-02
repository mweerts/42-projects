/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_listener.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 21:48:09 by llebugle          #+#    #+#             */
/*   Updated: 2025/06/27 21:48:11 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_LISTENER_HPP
#define HTTP_LISTENER_HPP

#include <netinet/in.h>
#include <unistd.h>

#include <string>

#include "server_config.hpp"

namespace http {
class Listener {
   public:
    explicit Listener(const Server& config) : config_(config), listen_fd_(-1) {}

    ~Listener() {
        if (listen_fd_ >= 0) {
            close(listen_fd_);
        }
    }

    bool Initialize();  // Create socket, bind and listen
    int  GetListenSocket() const;

    void AcceptAndHandle();  // Only temporary for testing

   private:
    Server config_;
    int    listen_fd_;

   private:
    void        HandleRequest(int client_fd);
    bool        BindAddress();
    std::string BuildHttpResponse();

    Listener(const Listener& other) {
        (void)other;
    };
    Listener& operator=(const Listener& other) {
        (void)other;
        return *this;
    };
};
}  // namespace http

#endif
