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
#include "config/server_config.hpp"
#include "core/web_server.hpp"
#include "tests.cpp"

int main() {
    Logger::setLevel(LOG_LEVEL_DEBUG);
    Logger::enableColors(true);

    Logger::info() << "... initializing config ...";

    Config config = testConfig();
    // Config config = parseConf();

    WebServer web_server(config);
    
    Logger::info() << "... initializing servers ...";
    if (!web_server.Start()) {
        return 1;
    }

    const int MAX_RESTARTS = 3;
    const int RESTART_DELAY_SEC = 1;
    
    for (int i = 0; i < MAX_RESTARTS; ++i) {
        try {
            web_server.Run();
            break;
        } catch (const std::exception& e) {
            Logger::critical() << e.what();
            if (i < MAX_RESTARTS - 1) {
                Logger::info() << "Restarting server in " << RESTART_DELAY_SEC
                               << " seconds...";
                sleep(RESTART_DELAY_SEC);
            } else {
                Logger::critical() << "Max restart attempts reached. Exiting.";
                return 1;
            }
        }
    }
    return 0;
}
