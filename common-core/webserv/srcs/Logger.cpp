/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 19:02:07 by llebugle          #+#    #+#             */
/*   Updated: 2025/05/09 19:02:07 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Logger.hpp"

#include <iostream>
#include <string>
#include <time.h>

const char* Logger::RESET = "\033[0m";
const char* Logger::GREEN = "\033[32m";
const char* Logger::BLUE = "\033[34m";
const char* Logger::YELLOW = "\033[33m";
const char* Logger::RED = "\033[31m";
const char* Logger::PURPLE = "\033[36m";

LogLevel Logger::_currentLevel = LOG_LEVEL_DEBUG;
bool     Logger::_useColors = true;
bool     Logger::_useTimestamps = false;

LogLevel Logger::getLevel() {
    return _currentLevel;
}
void Logger::setLevel(LogLevel level) {
    _currentLevel = level;
}
void Logger::enableColors(bool enableColors) {
    _useColors = enableColors;
}

void Logger::enableTimestamps(bool enable) {
    _useTimestamps = enable;
}

const std::string Logger::getLevelName(LogLevel level) {
    switch (level) {
        case LOG_LEVEL_DEBUG: return "DEBUG";
        case LOG_LEVEL_INFO: return "INFO";
        case LOG_LEVEL_WARNING: return "WARNING";
        case LOG_LEVEL_ERROR: return "ERROR";
        case LOG_LEVEL_NONE: return "NONE";
        default: return "UNKNOWN";
    }
}

const std::string Logger::getCurrentTimestamp() {
    time_t rawTime;
    struct tm* timeInfo;
    char buffer[80];
    
    time(&rawTime);
    timeInfo = localtime(&rawTime);
    
    // Format: "2024-01-11 14:30:25"
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);
    
    return std::string(buffer);
}

void Logger::log(LogLevel level, const std::string& message) {
    if (level < _currentLevel || level >= LOG_LEVEL_NONE || message.empty()) {
        return;
    }

    std::string timestamp;
    
    if (_useTimestamps) {
        timestamp += "[" + getCurrentTimestamp() + "]" + " ";
    }
    
    if (_useColors) {
        std::cout << timestamp;
        switch (level) {
            case LOG_LEVEL_DEBUG: {
                std::cout << PURPLE;
                break;
            }
            case LOG_LEVEL_INFO: {
                std::cout << BLUE;
                break;
            }
            case LOG_LEVEL_WARNING: {
                std::cout << YELLOW;
                break;
            }
            case LOG_LEVEL_ERROR: {
                std::cout << RED;
                break;
            }
            default: break;
        }
        std::cout << "[" << getLevelName(level) << "] " << RESET << message;
    } else {
        std::cout << "[" << getLevelName(level) << "] " << message;
    }
    std::cout << "\n";
}

LogStream Logger::debug() {
    return LogStream(LOG_LEVEL_DEBUG);
}

LogStream Logger::info() {
    return LogStream(LOG_LEVEL_INFO);
}

LogStream Logger::warning() {
    return LogStream(LOG_LEVEL_WARNING);
}

LogStream Logger::error() {
    return LogStream(LOG_LEVEL_ERROR);
}

void Logger::debug(const std::string& message) {
    log(LOG_LEVEL_DEBUG, message);
}

void Logger::info(const std::string& msg) {
    log(LOG_LEVEL_INFO, msg);
}
void Logger::warning(const std::string& msg) {
    log(LOG_LEVEL_WARNING, msg);
}
void Logger::error(const std::string& msg) {
    log(LOG_LEVEL_ERROR, msg);
}
