/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_status.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 20:11:19 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/18 20:11:20 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_status.hpp"

#include <cstring>
#include <iomanip>
#include <sstream>

ServerStatus::ServerStatus()
    : start_time_(time(NULL)),
      total_connections_(0),
      active_connections_(0),
      total_requests_(0),
      requests_last_minute_(0),
      last_minute_start_(time(NULL)),
      requests_this_minute_(0) {}

ServerStatus::~ServerStatus() {}

ServerStatus& ServerStatus::getInstance() {
    static ServerStatus instance;
    return instance;
}

void ServerStatus::onConnectionEstablished() {
    total_connections_++;
    active_connections_++;
}

void ServerStatus::onConnectionClosed() {
    if (active_connections_ > 0) {
        active_connections_--;
    }
}

void ServerStatus::onRequestProcessed() {
    total_requests_++;
    requests_this_minute_++;

	// update minute stats
	time_t current_time = time(NULL);
    if (current_time - last_minute_start_ >= 60) {
        requests_last_minute_ = requests_this_minute_;
        requests_this_minute_ = 0;
        last_minute_start_ = current_time;
    }
}

std::string ServerStatus::getJson() const {
    std::ostringstream oss;

    oss << "{";
    oss << "\"server_info\": {";
    oss << "\"start_time\": " << start_time_ << ",";
    oss << "\"uptime_seconds\": " << (time(NULL) - start_time_) << ",";
    oss << "\"uptime_formatted\": \"" << formatUptime(time(NULL) - start_time_)
        << "\"";
    oss << "},";

    oss << "\"connections\": {";
    oss << "\"total\": " << total_connections_ << ",";
    oss << "\"active\": " << active_connections_;
    oss << "},";

    oss << "\"requests\": {";
    oss << "\"total\": " << total_requests_ << ",";
    oss << "\"last_minute\": " << requests_last_minute_;
    oss << "}";

    oss << "}";

    return oss.str();
}

std::string ServerStatus::formatUptime(time_t seconds) const {
    int days = seconds / 86400;
    int hours = (seconds % 86400) / 3600;
    int minutes = (seconds % 3600) / 60;
    int secs = seconds % 60;

    std::ostringstream oss;
    if (days > 0)
        oss << days << "d ";
    if (hours > 0 || days > 0)
        oss << hours << "h ";
    if (minutes > 0 || hours > 0 || days > 0)
        oss << minutes << "m ";
    oss << secs << "s";

    return oss.str();
}
