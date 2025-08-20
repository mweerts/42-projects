/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 19:02:07 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/20 18:33:46 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

#include <time.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <string>

static const size_t MAX_LOG_SIZE = 1024 * 1024;  // 1MB

const char* Logger::RESET = "\033[0m";
const char* Logger::GREEN = "\033[32m";
const char* Logger::BLUE = "\033[34m";
const char* Logger::YELLOW = "\033[33m";
const char* Logger::RED = "\033[31m";
const char* Logger::CRITICAL = "\033[31m";
const char* Logger::PURPLE = "\033[36m";

LogLevel      Logger::_currentLevel = LOG_LEVEL_DEBUG;
bool          Logger::_useColors = true;
bool          Logger::_useTimestamps = false;
bool          Logger::_logToFile = false;
std::ofstream Logger::_logFile;
std::string   Logger::_logFilename;

LogLevel Logger::getLevel() {
    return _currentLevel;
}
void Logger::setLevel(LogLevel level) {
    _currentLevel = level;
}

bool Logger::setLogFile(const std::string& filename) {
    if (_logToFile) {
        _logFile.close();
    }
    _logFile.open(filename.c_str(), std::ios::app);
    if (!_logFile.is_open()) {
        return false;
    }
    _logToFile = true;
    _logFilename = filename;
    return true;
}

void Logger::enableColors(bool enableColors) {
    _useColors = enableColors;
}

void Logger::enableTimestamps(bool enable) {
    _useTimestamps = enable;
}

std::string& Logger::getLogFilename() {
    return _logFilename;
}

const std::string Logger::getLevelName(LogLevel level) {
    switch (level) {
        case LOG_LEVEL_DEBUG: return "DEBUG";
        case LOG_LEVEL_INFO: return "INFO";
        case LOG_LEVEL_WARNING: return "WARNING";
        case LOG_LEVEL_ERROR: return "ERROR";
        case LOG_LEVEL_CRITICAL: return "CRITICAL";
        case LOG_LEVEL_NONE: return "NONE";
        default: return "UNKNOWN";
    }
}

const std::string Logger::getCurrentHour() {
    time_t     rawTime;
    struct tm* timeInfo;
    char       buffer[80];

    time(&rawTime);
    timeInfo = localtime(&rawTime);
    strftime(buffer, sizeof(buffer), "%H:%M:%S", timeInfo);
    return std::string(buffer);
}

const std::string Logger::getCurrentTimestamp() {
    time_t     rawTime;
    struct tm* timeInfo;
    char       buffer[80];

    time(&rawTime);
    timeInfo = localtime(&rawTime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);
    return std::string(buffer);
}

static const char* getColor(LogLevel level) {
    switch (level) {
        case LOG_LEVEL_DEBUG: return Logger::PURPLE;
        case LOG_LEVEL_INFO: return Logger::GREEN;
        case LOG_LEVEL_WARNING: return Logger::YELLOW;
        case LOG_LEVEL_ERROR: return Logger::RED;
        case LOG_LEVEL_CRITICAL: return Logger::RED;
        default: return Logger::RESET;
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    if (level < _currentLevel || level >= LOG_LEVEL_NONE || message.empty()) {
        return;
    }

    std::stringstream logMessage;
    std::string       color = RESET;
    std::string       level_prefix;
    level_prefix = "[" + getLevelName(level) + "] ";

    if (_logToFile && _logFile.is_open()) {
        _logFile.seekp(0, std::ios::end);
        if (_logFile.tellp() >
            static_cast<std::streampos>(MAX_LOG_SIZE)) {  // 10MB
            _logFile.close();
            _logFile.open(_logFilename.c_str(), std::ios::trunc);
        }
    }

    if (_useColors)
        color = getColor(level);

    std::string timestamp = "[" + getCurrentHour() + "]" + " ";

    if (_logToFile) {
        if (!_logFile.is_open() || _logFile.fail() ||
            access(_logFilename.c_str(), R_OK | W_OK) != 0) {
            _logFile.close();
            _logFile.open(_logFilename.c_str(), std::ios::app);
        }
        if (!_logFile.is_open()) {
            std::cout << "[ERROR] Failed to reopen log file: " << _logFilename
                      << "\n";
            return;
        }
        _logFile << timestamp << level_prefix << message << "\n";
        _logFile.flush();
    }
    std::cout << color << (_useTimestamps ? timestamp : "") << level_prefix
              << RESET << message << "\n";
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

LogStream Logger::critical() {
    return LogStream(LOG_LEVEL_CRITICAL);
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

void Logger::critical(const std::string& msg) {
    log(LOG_LEVEL_ERROR, msg);
}

void Logger::cleanup() {
    if (_logToFile && _logFile.is_open()) {
        _logFile.flush();
        _logFile.close();
    }
}
