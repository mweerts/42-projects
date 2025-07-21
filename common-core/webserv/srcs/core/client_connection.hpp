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
#include <fstream>
#include <string>

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

class ServerConfig;
class RequestHandler;

class ClientConnection {
   public:
    enum State {
        READING_REQUEST,
        READING_BODY,
        READING_CHUNKED,
        PROCESSING_REQUEST,
        WRITING_RESPONSE,
        ERROR,
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

    int   GetSocketFd() const;
    State GetState() const;

    void Close();

   private:
    static size_t       request_counter_;
    static const size_t BUFFER_SIZE = 4096;

    int                 socket_fd_;
    const ServerConfig& server_config_;
    time_t              last_activity_;
    std::string         request_buffer_;

    // Request handling
    RequestHandler* request_handler_;
    HttpRequest     current_request_;
    bool            request_ready_;

    // File storage
    std::string   request_file_path_;
    std::ofstream request_file_;
    std::string   chunked_file_path_;
    std::ofstream chunked_file_;
    size_t        request_size_;
    bool          chunked_complete_;

    // Response handling
    std::string response_buffer_;
    size_t      bytes_sent_;
    bool        response_complete_;

    // State management
    State state_;
    bool  keep_alive_;

    bool is_closed_;
    char read_buffer_[BUFFER_SIZE];

   private:
    void UpdateActivity();
    bool ReadRequest();
    bool HandleWrite();

    // FILE MANAGEMENT
    std::string getRequestFilePath();
    bool        saveRequestToFile(const char* buffer);

    // bool IsCompleteHTTPRequest() const;
    // void PrepareHTTPResponse();
};

#endif
