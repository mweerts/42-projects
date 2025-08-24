/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <lucas.lebugle@student.s19.be>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 00:00:00 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/21 13:03:57 by jfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <cstring>
#include <iostream>

#include "Logger.hpp"
#include "core/web_server.hpp"
#include "handlers/cgi_handler.hpp"
#include "parsing/GlobalConfig.hpp"

void initializeCgiBin(const std::vector<ServerConfig>& servers) {
    Logger::info() << "Initializing CGI bin...";

    for (size_t i = 0; i < servers.size(); i++) {
        const CgiBin& cgiBin = servers[i].getCgiBin();
        if (!cgiBin.getExt().empty() && !cgiBin.getPath().empty()) {
            CgiHandler::initializeCgiBin(cgiBin);
        }
    }
}

int main(int argc, char* argv[]) {
    Logger::setLevel(LOG_LEVEL_INFO);
    std::string config_file = "config/max.conf";
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-c" && i + 1 < argc) {
            config_file = static_cast<std::string>(argv[i + 1]);
            Logger::info() << "Using config file: " << config_file;
        } else if (std::string(argv[i]) == "-v")  {
            Logger::setLevel(LOG_LEVEL_DEBUG);
        }
    }

    try {
        GlobalConfig config;

        if (!config.loadConfig(config_file)) {
            Logger::error() << "Failed to load configuration file";
            return 1;
        }

        initializeCgiBin(config.getServers());
        WebServer server(config);
        if (!server.Start()) {
            Logger::error() << "Failed to start server";
            return 1;
        }
        server.Run();  // TODO: add a loop to restart the server if it crashes
                       // # at the end of project #

    } catch (const std::exception& e) {
        Logger::critical() << "Fatal error: " << e.what();
        return 1;
    }

    return 0;
}
