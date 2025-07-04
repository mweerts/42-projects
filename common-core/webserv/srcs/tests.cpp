/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 16:59:11 by llebugle          #+#    #+#             */
/*   Updated: 2025/06/29 16:59:18 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_config.hpp"

namespace {
int GetNextTestPort() {
    static int current_port = 8080;
    return current_port++;
}
};  // namespace

void testVirtualServers(Config& config) {
    ServerConfig main_server;
    main_server.name = "main_site";
    main_server.host = "localhost";
    main_server.port = GetNextTestPort();
    main_server.document_root = "./www";
    main_server.headers["ServerConfig"] = "MainSite/1.0";
    config.servers.push_back(main_server);

    ServerConfig api_server;
    api_server.name = "api_server";
    api_server.host = "127.0.0.1";
    api_server.port = GetNextTestPort();
    api_server.document_root = "./api";
    api_server.headers["ServerConfig"] = "API/1.0";
    api_server.headers["Content-Type"] = "application/json";
    config.servers.push_back(api_server);

    ServerConfig static_server;
    static_server.name = "static_files";
    static_server.host = "localhost";
    static_server.port = GetNextTestPort();
    static_server.document_root = "./static";
    static_server.headers["Cache-Control"] = "max-age=3600";
    config.servers.push_back(static_server);
}

void testAddressAlreadyInUse(Config& config) {
    ServerConfig api_server_2;
    api_server_2.name = "api_server_2";
    api_server_2.host = "localhost";
    api_server_2.port = 8081;
    api_server_2.document_root = "./api";
    api_server_2.headers["ServerConfig"] = "API/1.0";
    api_server_2.headers["Content-Type"] = "application/json";
    config.servers.push_back(api_server_2);
}

void TestPrivilegedPort(Config& config) {
    ServerConfig privileged;
    privileged.name = "privileged-ServerConfig";
    privileged.port = 80;  // Requires root privileges
    privileged.host = "127.0.0.1";
    config.servers.push_back(privileged);
}

void TestInvalidIp(Config& config) {
    ServerConfig invalid;
    invalid.name = "invalid-ServerConfig";
    invalid.port = GetNextTestPort();
    invalid.host = "1257.0.0.1";
    config.servers.push_back(invalid);
}

Config testConfig() {
    Config config;

    testVirtualServers(config);
#ifdef TEST
    testAddressAlreadyInUse(config);
    TestPrivilegedPort(config);
    TestInvalidIp(config);
#endif

    return config;
}
