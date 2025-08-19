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

#include <map>
#include <string>
#include <ctime>

class ServerStatus {
public:
    static ServerStatus& getInstance();
    
    void onConnectionEstablished();
    void onConnectionClosed();
    void onRequestProcessed();
	
    std::string getJson() const;
    
    time_t getStartTime() const { return start_time_; }
    size_t getTotalConnections() const { return total_connections_; }
    size_t getActiveConnections() const { return active_connections_; }
    size_t getTotalRequests() const { return total_requests_; }
    size_t getRequestsLastMinute() const { return requests_last_minute_; }
    
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
    
    std::string formatUptime(time_t seconds) const;
    
    ServerStatus(const ServerStatus&);
    ServerStatus& operator=(const ServerStatus&);
};

#endif
