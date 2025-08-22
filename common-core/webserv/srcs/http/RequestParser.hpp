/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:34:42 by llebugle          #+#    #+#             */
/*   Updated: 2025/07/21 18:34:43 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_PARSER_HPP
#define REQUEST_PARSER_HPP

#include <fstream>
#include <map>
#include <string>

#include "HttpRequest.hpp"
#include "http_status_code.hpp"

class ServerConfig;

class RequestParser {
   public:
    enum Status { NEED_MORE_DATA, COMPLETE, ERROR };

    RequestParser(HttpRequest& request, const ServerConfig& config);
    ~RequestParser();

    Status parse(const char* buffer, size_t buffer_size);

    // Status checking methods
    bool isComplete() const;
    bool needMoreData() const;
    bool hasError() const;

    StatusCode  getStatusCode() const;
    std::string getStatusMessage() const;

    void cleanup();

   private:
    enum ParsingPhase {
        REQUEST_LINE,
        HEADERS,
        BODY,
        COMPLETE_PHASE,
        ERROR_PHASE
    };

    static const size_t HEADER_BUFFER_SIZE = 16384; // 16kb
    static size_t       request_counter_; // to create unique request file names

    // Core members
    HttpRequest&        request_;
    const ServerConfig& server_config_;

    StatusCode  status_code_;
    std::string status_message_;

    // Parsing state
    ParsingPhase current_phase_;
    std::string  header_buffer_;
    size_t       headers_end_pos_;
    size_t       request_size_;

	std::string req_buffer_;
	
    // File management
    std::string   req_filename_;
    std::ofstream request_file_;

    Status parseRequestLine();
    Status parseHeaders();
    Status parseBody();

    void setError(StatusCode status_code, const std::string& message = "");

    std::string readFromFile(size_t start_pos = 0, size_t length = 0) const;
    std::string createRequestFilePath();
    bool        saveToFile(const char* buffer, size_t size);
    bool        validateAndSetRequestLine(const std::string& line);
    bool        extractHeaders(const std::string& headers_str);
    bool        findHeadersEnd();
    bool        hasBody() const;
};

#endif
