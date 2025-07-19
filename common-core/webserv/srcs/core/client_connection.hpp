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

// TODO: refactor this file

#ifndef CLIENT_CONNECTION_HPP
#define CLIENT_CONNECTION_HPP

#include <ctime>
#include <string>

class ServerConfig;

class ClientConnection {
   public:
    enum State {
        READING_REQUEST,
        PROCESSING_REQUEST,
        WRITING_RESPONSE,
        KEEP_ALIVE,
        CLOSING
    };

   public:
    explicit ClientConnection(int socket_fd, const ServerConfig& server_config);
    ~ClientConnection() {
        Close();
    };

    bool HandleEvent(short revents);

    // State queries
    bool NeedsToRead() const;
    bool NeedsToWrite() const;
    bool ShouldClose() const;
    bool IsTimedOut(int timeout_seconds = 60) const;  // default nginx

    int   GetSocket() const;
    State GetState() const;

    void Close();

   private:
    static const size_t BUFFER_SIZE = 4096;

    int                 socket_fd_;
    const ServerConfig& server_config_;
    State               state_;
    bool                keep_alive_;
    time_t              last_activity_;
    std::string         request_buffer_;
    // std::string         response_buffer_;
    size_t              bytes_sent_;
    bool                is_closed_;
    char                read_buffer_[BUFFER_SIZE];

   private:
    void UpdateActivity();
    bool HandleRead();
    bool HandleWrite();
    // bool IsCompleteHTTPRequest() const;
    // void PrepareHTTPResponse();
};

#endif
