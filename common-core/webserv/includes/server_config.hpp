/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 20:53:14 by llebugle          #+#    #+#             */
/*   Updated: 2025/06/27 20:53:16 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <map>
#include <string>
#include <vector>

struct ServerConfig {
    static const size_t DEFAULT_MAX_HEADER_SIZE = 8192;   // 8KB (nginx default)
    static const size_t DEFAULT_MAX_REQUEST_LINE = 4096;  // URL + method
    static const size_t DEFAULT_MAX_BODY_SIZE = 1024 * 1024 * 10;  // 10mb

    int         port;
    std::string name;
    std::string host;
    std::string document_root;

    std::map<std::string, std::string> headers;

    size_t client_max_header_size;
    size_t client_max_request_line;
    size_t client_max_body_size;

    ServerConfig()
        : host("localhost"),
          document_root("www"),
          client_max_header_size(DEFAULT_MAX_HEADER_SIZE),
          client_max_request_line(DEFAULT_MAX_REQUEST_LINE),
          client_max_body_size(DEFAULT_MAX_BODY_SIZE) {};
};

struct Config {
    std::vector<ServerConfig> servers;
};

#endif
