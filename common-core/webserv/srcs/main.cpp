/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 18:24:28 by llebugle          #+#    #+#             */
/*   Updated: 2025/06/25 18:24:29 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include <exception>

#include "Logger.hpp"
#include "RequestHandler.hpp"
#include "core/web_server.hpp"
#include "parsing/include/ConfigProcessor.hpp"
#include "server_config.hpp"
#include "parsing/src/ConfigGett.cpp"

Config testConfig();

int main() {
    Logger::setLevel(LOG_LEVEL_DEBUG);
    Logger::enableColors(true);

    Logger::info() << "... Initializing config ...";


    ConfigProcessor conf("../config/default.conf");

    if (conf.tokenize()) {
        return 1;
    }

    // Config config = testConfig();
    ConfigGett config = ConfigGett(conf);
    WebServer web_server(config);

    Logger::info() << "... Initializing servers ...";
    if (!web_server.Start()) {
        return 1;
    }

    while (true) {
        try {
            web_server.Run();
            break;
        } catch (const std::exception& e) {
            Logger::critical() << "Server crashed: " << e.what();
            web_server.Reset();
            Logger::info() << "Restarting server...";
            sleep(1);
        }
    }
    return 0;
}
