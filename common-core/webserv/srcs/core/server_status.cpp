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

#include <sys/time.h>

#include <cstring>
#include <iomanip>
#include <sstream>
#include <vector>

ServerStatus::ServerStatus()
    : start_time_(time(NULL)),
      total_connections_(0),
      active_connections_(0),
      total_requests_(0),
      requests_last_minute_(0),
      last_minute_start_(time(NULL)),
      requests_this_minute_(0),
      request_start_us_(0) {
    response_times_.reserve(MAX_RESPONSE_TIMES);
}

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

static long long now_us() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000000LL + (long long)tv.tv_usec;
}

void ServerStatus::onRequestStarted() {
    request_start_us_ = now_us();
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
    if (request_start_us_ != 0) {
        long long end_us = now_us();
        double response_time = static_cast<double>(end_us - request_start_us_) / 1000000.0;
        addResponseTime(response_time);
        request_start_us_ = 0;
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
    oss << "\"last_minute\": " << requests_last_minute_ << ",";
    oss << "\"average_response_time_ms\": " << std::fixed
        << std::setprecision(2) << getAverageResponseTime() * 1000 << ",";
    oss << "\"last_response_time_ms\": " << std::fixed << std::setprecision(2)
        << getLastResponseTime() * 1000;
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

void ServerStatus::addResponseTime(double response_time) {
    response_times_.push_back(response_time);
    if (response_times_.size() > MAX_RESPONSE_TIMES) {
        response_times_.erase(response_times_.begin());
    }
}

double ServerStatus::getAverageResponseTime() const {
    if (response_times_.empty()) {
        return 0.0;
    }

    double sum = 0.0;
    for (std::vector<double>::const_iterator it = response_times_.begin();
         it != response_times_.end(); ++it) {
        sum += *it;
    }

    return sum / response_times_.size();
}

double ServerStatus::getLastResponseTime() const {
    if (response_times_.empty()) {
        return 0.0;
    }
    return response_times_.back();
}

time_t ServerStatus::getStartTime() const {
    return start_time_;
}

size_t ServerStatus::getTotalConnections() const {
    return total_connections_;
}

size_t ServerStatus::getActiveConnections() const {
    return active_connections_;
}

size_t ServerStatus::getTotalRequests() const {
    return total_requests_;
}

size_t ServerStatus::getRequestsLastMinute() const {
    return requests_last_minute_;
}
