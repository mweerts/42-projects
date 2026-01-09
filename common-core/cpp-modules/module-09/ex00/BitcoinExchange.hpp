#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <ctime>
#include <map>
#include <string>

class BitcoinExchange {
   public:
    BitcoinExchange();
    BitcoinExchange(const BitcoinExchange& other);
    BitcoinExchange& operator=(const BitcoinExchange& other);
    ~BitcoinExchange();

    void processInputFile(const std::string& inputFile);

   private:
    std::map<time_t, double> _exchangeRates;

    void        loadExchangeRates(const std::string& dataFile);
    time_t      parseDate(const std::string& dateStr);
    std::string timeToDateString(time_t timestamp);
    bool        isCorrectDate(int year, int month, int day);
};

#endif