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

#include <sys/poll.h>

#include <ctime>
#include <string>
#include <vector>

#include "../handlers/response_streamer.hpp"
#include "../http/HttpRequest.hpp"
#include "../http/HttpResponse.hpp"
#include "../http/RequestParser.hpp"

class ServerConfig;
class RequestHandler;
class CgiHandler;

class ClientConnection {
   public:
    enum State { READING_REQUEST, WRITING_RESPONSE, ERROR };

   public:
    explicit ClientConnection(int socket_fd, const ServerConfig& server_config);
    ~ClientConnection();

    bool HandleEvent(short revents);

    bool NeedsToRead() const;
    bool NeedsToWrite() const;
    bool ShouldClose() const;
    bool IsTimedOut(int timeout_seconds = 30) const;

    int   GetSocketFd() const;
    State GetState() const;
    
	void Close();

    // Expose aux fds for polling (files, cgi pipes)
    std::vector<pollfd> GetAuxPollFds() const;
    void                HandleAuxEvent(int fd, short revents);

   private:
    static const size_t BUFFER_SIZE = 16384;  // 16kb

    char                read_buffer_[BUFFER_SIZE];
    int                 socket_fd_;
    const ServerConfig& server_config_;
    time_t              last_activity_;

    RequestParser*  request_parser_;
    RequestHandler* request_handler_;
    HttpRequest     current_request_;

    // State management
    State state_;
    bool  is_closed_;

    bool request_ready_;

    ResponseStreamer response_streamer_;

   private:
    void UpdateActivity();
    bool HandleRead();
    bool HandleWrite();
    bool finalizeResponse();
};

#endif
