#include "ScalarConverter.hpp"

#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

ScalarConverter::ScalarConverter() {}

ScalarConverter::ScalarConverter(const ScalarConverter& other) {
    (void)other;
}

ScalarConverter& ScalarConverter::operator=(const ScalarConverter& other) {
    (void)other;
    return *this;
}

ScalarConverter::~ScalarConverter() {}

static void printChar(char c, bool isPossible) {
    if (!isPossible) {
        std::cout << "char: impossible" << std::endl;
        return;
    }
    if (isprint(c))
        std::cout << "char: '" << c << "'" << std::endl;
    else
        std::cout << "char: Non displayable" << std::endl;
}

static void printInt(int i, bool isPossible) {
    if (isPossible)
        std::cout << "int: " << i << std::endl;
    else
        std::cout << "int: impossible" << std::endl;
}

static void printFloat(float f, bool isPossible) {
    if (isPossible)
        std::cout << "float: " << std::setprecision(1) << std::fixed << f << "f"
                  << std::endl;
    else
        std::cout << "float: impossible" << std::endl;
}

static void printDouble(double d, bool isPossible) {
    if (isPossible)
        std::cout << "double: " << std::setprecision(1) << std::fixed << d
                  << std::endl;
    else
        std::cout << "double: impossible" << std::endl;
}

static bool strIsChar(const std::string& str) {
    return (str.length() == 1 && !isdigit(str[0]));
}

static bool strIsFloat(const std::string& str) {
    if (str[str.length() - 1] != 'f')
        return false;
    if (str == "inff" || str == "-inff" || str == "+inff" || str == "nanf")
        return true;

    std::istringstream iss(str.substr(0, str.length() - 1));
    float              val;

    iss >> val;
    if (iss.fail() || !iss.eof()) {
        return false;
    }
    return true;
}

static bool strIsDouble(const std::string& str) {
    if (str == "inf" || str == "-inf" || str == "+inf" || str == "nan")
        return true;

    std::istringstream iss(str);
    double             val;

    iss >> val;
    if (iss.fail() || !iss.eof()) {
        return false;
    }
    return true;
}

static bool strIsInt(const std::string& str) {
    std::istringstream iss(str);
    int                val;

    iss >> val;
    if (iss.fail() || !iss.eof()) {
        return false;
    }
    return true;
}

static bool strIsFloatLiteral(const std::string& str) {
    return (str == "inff" || str == "-inff" || str == "+inff" || str == "nanf");
}

static bool strIsDoubleLiteral(const std::string& str) {
    return (str == "inf" || str == "-inf" || str == "+inf" || str == "nan");
}

ScalarConverter::t_value ScalarConverter::parseChar(const std::string& str) {
    t_value ret;
    char    val;
    if (str.length() != 1 || isdigit(str[0])) {
        return ret;
    }

    val = str[0];

    ret.c = val;
    ret.isCharPossible = true;

    ret.i = static_cast<int>(val);
    ret.isIntPossible = true;

    ret.f = static_cast<float>(val);
    ret.isFloatPossible = true;

    ret.d = static_cast<double>(val);
    ret.isDoublePossible = true;

    return ret;
}

ScalarConverter::t_value ScalarConverter::parseFloat(const std::string& str) {
    t_value ret;
    float   val = 0.0;

    if (strIsFloatLiteral(str)) {
        if (str == "inff" || str == "+inff")
            val = std::numeric_limits<float>::infinity();
        else if (str == "-inff")
            val = std::numeric_limits<float>::infinity() * -1;
        else if (str == "nanf")
            val = std::numeric_limits<float>::quiet_NaN();
    } else {
        std::string literal = str;
        if (literal[literal.length() - 1] == 'f')
            literal = literal.substr(0, literal.length() - 1);
        std::istringstream iss(literal);
        iss >> val;

        if (iss.fail() || val > std::numeric_limits<float>::max() ||
            val < -std::numeric_limits<float>::max()) {
            return ret;
        }
    }
    ret.f = val;
    ret.isFloatPossible = true;

    if (val >= 0 && val <= 127) {
        ret.c = static_cast<char>(val);
        ret.isCharPossible = true;
    }

    if (val >= -std::numeric_limits<int>::max() &&
        val <= std::numeric_limits<int>::max()) {
        ret.i = static_cast<int>(val);
        ret.isIntPossible = true;
    }
    if (val >= -std::numeric_limits<double>::max() &&
        val <= std::numeric_limits<double>::max()) {
        ret.d = static_cast<double>(val);
        ret.isDoublePossible = true;
    }

    return ret;
}

ScalarConverter::t_value ScalarConverter::parseDouble(const std::string& str) {
    t_value ret;
    double  val = 0.0;

    if (strIsDoubleLiteral(str)) {
        if (str == "inf" || str == "+inf")
            val = std::numeric_limits<double>::infinity();
        else if (str == "-inf")
            val = std::numeric_limits<double>::infinity() * -1;
        else if (str == "nan")
            val = std::numeric_limits<double>::quiet_NaN();
    } else {
        std::istringstream iss(str);
        iss >> val;
        if (iss.fail() || val > std::numeric_limits<double>::max() ||
            val < -std::numeric_limits<double>::max()) {
            return ret;
        }
    }
    ret.d = val;
    ret.isDoublePossible = true;

    if (val >= 0 && val <= 127) {
        ret.c = static_cast<char>(val);
        ret.isCharPossible = true;
    }

    if (val >= -std::numeric_limits<int>::max() &&
        val <= std::numeric_limits<int>::max()) {
        ret.i = static_cast<int>(val);
        ret.isIntPossible = true;
    }

    if ((val >= -std::numeric_limits<float>::max() &&
            val <= std::numeric_limits<float>::max()) ||
        strIsDoubleLiteral(str)) {
        ret.f = static_cast<float>(val);
        ret.isFloatPossible = true;
    }

    return ret;
}

ScalarConverter::t_value ScalarConverter::parseInt(const std::string& str) {
    t_value ret;
    int     val;

    std::istringstream iss(str);
    iss >> val;

    if (iss.fail()) {
        return ret;
    }
    ret.i = val;
    ret.isIntPossible = true;

    if (val >= 0 && val <= 127) {
        ret.c = static_cast<char>(val);
        ret.isCharPossible = true;
    }

    if (val >= -std::numeric_limits<float>::max() &&
        val <= std::numeric_limits<float>::max()) {
        ret.f = static_cast<float>(val);
        ret.isFloatPossible = true;
    }

    if (val >= -std::numeric_limits<double>::max() &&
        val <= std::numeric_limits<double>::max()) {
        ret.d = static_cast<double>(val);
        ret.isDoublePossible = true;
    }

    return ret;
}

void ScalarConverter::printResult(ScalarConverter::t_value val) {
    printChar(val.c, val.isCharPossible);
    printInt(val.i, val.isIntPossible);
    printFloat(val.f, val.isFloatPossible);
    printDouble(val.d, val.isDoublePossible);
}

void ScalarConverter::convert(const std::string& str) {
    if (strIsChar(str)) {
        printResult(parseChar(str));
    } else if (strIsInt(str)) {
        printResult(parseInt(str));
    } else if (strIsFloat(str)) {
        printResult(parseFloat(str));
    } else if (strIsDouble(str)) {
        printResult(parseDouble(str));
    } else {
        t_value val;
        printResult(val);
    }
}
