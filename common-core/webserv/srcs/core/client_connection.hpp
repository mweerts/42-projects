/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_connection.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 00:43:12 by llebugle          #+#    #+#             */
/*   Updated: 2025/06/30 00:43:15 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_CONNECTION_HPP
#define CLIENT_CONNECTION_HPP

#include <ctime>
#include <string>

#include "../http/RequestParser.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

class ServerConfig;
class RequestHandler;
class CgiHandler;

class ClientConnection {
   public:
    enum State {
        READING_REQUEST,
        WRITING_RESPONSE,
        CGI_PROCESSING,
        CLOSING,
        ERROR
    };

   public:
    explicit ClientConnection(int socket_fd, const ServerConfig& server_config);
    ~ClientConnection();

    bool HandleEvent(short revents);

    bool NeedsToRead() const;
    bool NeedsToWrite() const;
    bool ShouldClose() const;
    bool IsTimedOut(int timeout_seconds = 60) const;  // default nginx

    int   GetSocketFd() const;
    State GetState() const;

    int GetCgiInputPipe() const;
    int GetCgiOutputPipe() const;

    void Close();

   private:
    static const size_t BUFFER_SIZE = 4096;

    char                read_buffer_[BUFFER_SIZE];
    int                 socket_fd_;
    const ServerConfig& server_config_;
    time_t              last_activity_;

    RequestParser*  request_parser_;
    RequestHandler* request_handler_;
    HttpRequest     current_request_;
    CgiHandler*     cgi_handler_;

    bool request_ready_;

    // State management
    State state_;
    bool  is_closed_;

   private:
    void UpdateActivity();
    bool HandleRead();
    bool HandleWrite();
    bool HandleCgi();
    void cleanupCgi();
};

#endif
