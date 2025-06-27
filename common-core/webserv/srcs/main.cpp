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

#include "Logger.hpp"
#include "config/server_config.hpp"
#include "core/web_server.hpp"
#include "tests.cpp"
#include <signal.h>

int main() {
    Logger::setLevel(LOG_LEVEL_DEBUG);
    Logger::enableColors(true);

    Logger::info() << "... initializing config ...";
    Config config = testConfig();

    WebServer web_server(config);
    Logger::info() << "... initializing servers ...";
    if (!web_server.Start()) {
        return 1;
    }
    Logger::info() << "... Server running ...";
    web_server.Run();

    return 0;
}
