#ifndef SCALARCONVERTER_HPP
#define SCALARCONVERTER_HPP

#include <string>

class ScalarConverter {
   private:
    typedef struct s_value {
        char   c;
        int    i;
        float  f;
        double d;

        bool isCharPossible;
        bool isIntPossible;
        bool isFloatPossible;
        bool isDoublePossible;

        s_value()
            : c(0),
              i(0),
              f(0.0f),
              d(0.0),
              isCharPossible(false),
              isIntPossible(false),
              isFloatPossible(false),
              isDoublePossible(false) {}
    } t_value;

    ScalarConverter();
    ScalarConverter(const ScalarConverter& other);
    ScalarConverter& operator=(const ScalarConverter& other);
    ~ScalarConverter();

    static ScalarConverter::t_value parseChar(const std::string& str);
    static ScalarConverter::t_value parseFloat(const std::string& str);
    static ScalarConverter::t_value parseDouble(const std::string& str);
    static ScalarConverter::t_value parseInt(const std::string& str);

    static void printResult(ScalarConverter::t_value val);

   public:
    static void convert(const std::string& literal);
};

#endif