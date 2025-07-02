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

struct Server {
    int         port;
    std::string name;
    std::string host;
    std::string document_root;

    std::map<std::string, std::string> headers;
};

struct Config {
    std::vector<Server> servers;
};

#endif
