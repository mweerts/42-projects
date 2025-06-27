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

// class ConfigLoader {
//    public:
//     // Factory method - returns loaded config or nullopt on failure
//     // static std::optional<ServerConfig> LoadFromFile(
//     //     const std::filesystem::path& config_path);
//     // static std::optional<ServerConfig> LoadFromJson(
//     //     const std::string& json_content);

//    private:
//     static bool ValidateConfig(const Server& config);
// };

#endif
