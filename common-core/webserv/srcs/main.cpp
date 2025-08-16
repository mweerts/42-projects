/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <lucas.lebugle@student.s19.be>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 00:00:00 by llebugle          #+#    #+#             */
/*   Updated: 2025/07/19 02:10:08 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <iostream>

#include "Logger.hpp"
#include "core/web_server.hpp"
#include "parsing/GlobalConfig.hpp"

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cerr << "Usage: " << argv[0] << " [config_file]" << std::endl;
        return 1;
    }

    try {
        GlobalConfig config;

        if (argc == 2) {
            if (!config.loadConfig(argv[1])) {
                Logger::error() << "Failed to load configuration file";
                return 1;
            }
        } else {
            if (!config.loadConfig("config/default2.conf")) {
                Logger::error() << "Failed to load default configuration file";
                return 1;
            }
        }

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
