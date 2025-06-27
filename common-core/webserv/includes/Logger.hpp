/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 19:02:07 by llebugle          #+#    #+#             */
/*   Updated: 2025/05/09 19:02:07 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <sstream>
#include <string>

class LogStream;

enum LogLevel {
    LOG_LEVEL_DEBUG = 0,     // Most detailed
    LOG_LEVEL_INFO = 1,      // General information
    LOG_LEVEL_WARNING = 2,   // Warning messages
    LOG_LEVEL_ERROR = 3,     // Error messages
    LOG_LEVEL_CRITICAL = 4,  // Error messages
    LOG_LEVEL_NONE = 5       // No logging at all
};

class Logger {
   public:
    static const char* RESET;
    static const char* GREEN;
    static const char* BLUE;
    static const char* YELLOW;
    static const char* RED;
    static const char* CRITICAL;
    static const char* PURPLE;

    static LogLevel getLevel();
    static void     setLevel(LogLevel level);
    static void     enableColors(bool enableColors);
    static void     enableTimestamps(bool enable);

    static LogStream debug();
    static LogStream info();
    static LogStream warning();
    static LogStream error();
    static LogStream critical();

    static void debug(const std::string& message);
    static void info(const std::string& message);
    static void warning(const std::string& message);
    static void error(const std::string& message);
    static void critical(const std::string& message);

    static const std::string getLevelName(LogLevel level);

   private:
    friend class LogStream;

    static LogLevel _currentLevel;
    static bool     _useColors;
    static bool     _useTimestamps;

    static void log(LogLevel level, const std::string& message);
    static void log(LogLevel level, const std::stringstream& message);
    static const std::string getCurrentTimestamp();
};

class LogStream {
   public:
    LogStream(LogLevel level) : _level(level), _active(false) {
        if (Logger::getLevel() <= level && level < LOG_LEVEL_NONE) {
            _active = true;
        }
    }

    ~LogStream() {
        if (_active && !_buffer.empty()) {
            Logger::log(_level, _buffer);
        }
    }

    template <typename T>
    LogStream& operator<<(const T& value) {
        if (_active) {
            std::stringstream temp;
            temp << value;
            _buffer += temp.str();
        }
        return *this;
    }

   private:
    LogLevel    _level;
    std::string _buffer;
    bool        _active;
};

#define LOG_DEBUG(msg_expr)                      \
    if (Logger::getLevel() <= LOG_LEVEL_DEBUG) { \
        std::stringstream ss_debug;              \
        ss_debug << msg_expr;                    \
        Logger::debug(ss_debug.str());           \
    }

#define LOG_INFO(msg_expr)                      \
    if (Logger::getLevel() <= LOG_LEVEL_INFO) { \
        std::stringstream ss_info;              \
        ss_info << msg_expr;                    \
        Logger::info(ss_info.str());            \
    }

#define LOG_WARNING(msg_expr)                      \
    if (Logger::getLevel() <= LOG_LEVEL_WARNING) { \
        std::stringstream ss_warning;              \
        ss_warning << msg_expr;                    \
        Logger::warning(ss_warning.str());         \
    }

#define LOG_ERROR(msg_expr)                      \
    if (Logger::getLevel() <= LOG_LEVEL_ERROR) { \
        std::stringstream ss_error;              \
        ss_error << msg_expr;                    \
        Logger::error(ss_error.str());           \
    }

#define LOG_CRITICAL(msg_expr)                      \
    if (Logger::getLevel() <= LOG_LEVEL_CRITICAL) { \
        std::stringstream ss_error;                 \
        ss_error << msg_expr;                       \
        Logger::critical(ss_error.str());           \
    }

#endif
