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
    explicit ClientConnection(int socket_fd);
    ~ClientConnection() {
        Close();
    };

    // Event handlers
    // bool HandleRead();   // Returns false if connection should close
    // bool HandleWrite();  // Returns false if connection should close

    void Close();

    // State queries
    bool NeedsToRead() const;
    bool NeedsToWrite() const;
    bool ShouldClose() const;
    bool IsTimedOut(int timeout_seconds = 60) const;  // default nginx

    // Getters
    int   GetSocket() const;
    State GetState() const;

   private:
    int         socket_fd_;
    State       state_;
    time_t      last_activity_;
    std::string request_buffer_;
    std::string response_buffer_;
    size_t      bytes_sent_;
    bool        is_closed_;

   private:
    void UpdateActivity();
    // void ProcessHTTPRequest();
    // bool IsCompleteHTTPRequest() const;
    // void PrepareHTTPResponse();
};

#endif
