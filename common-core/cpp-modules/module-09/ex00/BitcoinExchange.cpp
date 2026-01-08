#include "BitcoinExchange.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

BitcoinExchange::BitcoinExchange() {
    try {
        loadExchangeRates("data.csv");
    } catch (const std::exception& e) {
        throw;
    }
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) {
    *this = other;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
    if (this != &other) {
        this->exchangeRates = other.exchangeRates;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

void BitcoinExchange::loadExchangeRates(const std::string& dataFile) {
    std::ifstream file(dataFile.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("Error: could not open file.");
    }

    std::string line;
    bool        firstLine = true;

    try {
        while (std::getline(file, line)) {
            if (firstLine) {
                firstLine = false;
                continue;
            }

            size_t commaPos = line.find(',');
            if (commaPos == std::string::npos) {
                throw std::runtime_error("Error: bad input => malformed line.");
            }

            std::string dateStr = line.substr(0, commaPos);
            std::string valueStr = line.substr(commaPos + 1);

            time_t timestamp = parseDate(dateStr);

            double             value;
            std::istringstream valueStream(valueStr);
            if (!(valueStream >> value)) {
                throw std::runtime_error("Error: bad input => invalid value.");
            }
            exchangeRates[timestamp] = value;
        }

        if (exchangeRates.empty()) {
            throw std::runtime_error("Error: no valid data loaded.");
        }
    } catch (...) {
        file.close();
        throw;
    }

    file.close();
}

void BitcoinExchange::processInputFile(const std::string& inputFile) {
    std::ifstream file(inputFile.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("Error: could not open file.");
    }

    std::string line;
    bool        firstLine = true;

    try {
        while (std::getline(file, line)) {
            if (firstLine) {
                firstLine = false;
                continue;
            }
            if (line.empty()) {
                continue;
            }

            size_t pipePos = line.find(" | ");
            if (pipePos == std::string::npos) {
                std::cerr << "Error: bad input => " << line << std::endl;
                continue;
            }

            std::string dateStr = line.substr(0, pipePos);
            std::string valueStr = line.substr(pipePos + 3);

            time_t timestamp;
            try {
                timestamp = parseDate(dateStr);
            } catch (const std::exception& e) {
                std::cerr << "Error: bad input => invalid date : " << dateStr
                          << std::endl;
                continue;
            }

            double             value;
            std::istringstream valueStream(valueStr);

            if (!(valueStream >> value)) {
                std::cerr << "Error: bad input => invalid value : " << valueStr
                          << std::endl;
                continue;
            }
            if (value < 0) {
                std::cerr << "Error: not a positive number." << std::endl;
                continue;
            }
            if (value > 1000) {
                std::cerr << "Error: too large number." << std::endl;
                continue;
            }

            std::map<time_t, double>::iterator it =
                exchangeRates.lower_bound(timestamp);
            if (it == exchangeRates.end() || it->first != timestamp) {
                if (it == exchangeRates.begin()) {
                    std::cerr << "Error: no earlier exchange rate available."
                              << std::endl;
                    continue;
                }
                --it;
            }

            double rate = it->second;
            double result = rate * value;

            std::cout << dateStr << " => " << value << " = " << result
                      << std::endl;
        }
    } catch (...) {
        file.close();
        throw;
    }

    file.close();
}

time_t BitcoinExchange::parseDate(const std::string& dateStr) {
    int                year, month, day;
    std::istringstream yearStream(dateStr.substr(0, 4));
    std::istringstream monthStream(dateStr.substr(5, 2));
    std::istringstream dayStream(dateStr.substr(8, 2));

    if (!(yearStream >> year) || !(monthStream >> month) ||
        !(dayStream >> day)) {
        throw std::runtime_error("Error: bad input => invalid date values.");
    }

    if (!isCorrectDate(year, month, day)) {
        throw std::runtime_error("Error: bad input => invalid date.");
    }

    struct tm timeinfo = {};
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_mday = day;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    timeinfo.tm_isdst = -1;
    time_t timestamp = mktime(&timeinfo);
    if (timestamp == -1) {
        throw std::runtime_error("Error: bad input => invalid date.");
    }
    return timestamp;
}

std::string BitcoinExchange::timeToDateString(time_t timestamp) {
    struct tm* timeinfo = localtime(&timestamp);
    char       buffer[11];

    strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);
    if (buffer[0] == '\0') {
        throw std::runtime_error("Error: could not format date string.");
    }
    return std::string(buffer);
}

bool BitcoinExchange::isCorrectDate(int year, int month, int day) {
    if (month < 1 || month > 12 || day < 1 || day > 31) {
        return false;
    }
    if (month == 2) {
        bool isLeapYear =
            (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        int maxDays = isLeapYear ? 29 : 28;
        if (day > maxDays) {
            return false;
        }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) {
            return false;
        }
    }
    return true;
}
