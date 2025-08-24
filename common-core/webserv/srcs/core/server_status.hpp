/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_status.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 20:11:26 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/18 20:11:34 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_STATUS_HPP
#define SERVER_STATUS_HPP

#include <ctime>
#include <map>
#include <string>

class ServerStatus {
   public:
    static ServerStatus& getInstance();

    void onConnectionEstablished();
    void onConnectionClosed();
    void onRequestProcessed();

    std::string getJson() const;

    time_t getStartTime() const;
    size_t getTotalConnections() const;
    size_t getActiveConnections() const;
    size_t getTotalRequests() const;
    size_t getRequestsLastMinute() const;

    void   onRequestStarted();
    double getAverageResponseTime() const;
    double getLastResponseTime() const;

   private:
    ServerStatus();  // singleton
    ~ServerStatus();

    time_t start_time_;
    size_t total_connections_;
    size_t active_connections_;
    size_t total_requests_;
    size_t requests_last_minute_;

    time_t last_minute_start_;
    size_t requests_this_minute_;

    static const size_t      MAX_RESPONSE_TIMES = 1000;
	long long request_start_us_;
    std::vector<double>      response_times_;

    void        addResponseTime(double response_time);
    std::string formatUptime(time_t seconds) const;

    ServerStatus(const ServerStatus&);
    ServerStatus& operator=(const ServerStatus&);
};

#endif
