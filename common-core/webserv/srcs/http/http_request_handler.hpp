/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_request_handler.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 21:23:58 by llebugle          #+#    #+#             */
/*   Updated: 2025/07/02 21:24:00 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST_HANDLER_HPP
#define HTTP_REQUEST_HANDLER_HPP

#include <map>
#include <string>

#include "server_config.hpp"

namespace http {
struct HttpRequest {
    std::string                        method;
    std::string                        uri;
    std::string                        version; // not sure about this
    std::map<std::string, std::string> headers;
    std::string                        body;

    HttpRequest() : version("1.1"){};
};

struct HttpResponse {
    int         status_code;
    std::string status_message;

    std::map<std::string, std::string> headers;
    std::string                        body;

    HttpResponse() : status_code(200), status_message("OK") {}

    std::string ToString() const; // TO IMPLEMENT
};

class RequestHandler {
   public:
    RequestHandler(const Server& server) : server_(server) {};
    ~RequestHandler() {};

    bool ParseRequest(const std::string& raw_request);  // TO IMPLEMENT
    void ProcessRequest();                              // TO IMPLEMENT

    HttpResponse& BuildResponse();  // TO IMPLEMENT

    // State queries
    bool IsRequestComplete() const;  // TO IMPLEMENT
    bool NeedsMoreData() const;      // TO IMPLEMENT

   private:
    const Server& server_;
    HttpRequest   request_;   // do i need to store this?
    HttpResponse  response_;  // do i need to store this?
    bool          request_complete_;
    bool          processing_complete_;

    // Helper methods for parsing
    void HandleMethod();                                  // TO IMPLEMENT
    void ParseRequestLine(const std::string& line);       // TO IMPLEMENT
    void ParseHeaders(const std::string& headers_block);  // TO IMPLEMENT
    void ParseBody(const std::string& body_block);        // TO IMPLEMENT

    // void GenerateErrorResponse(int status_code, const std::string& message);
    // std::string GenerateErrorResponse(int status_code);
};

}  // namespace http

#endif