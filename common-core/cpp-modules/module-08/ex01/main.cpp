#include <ctime>
#include <iostream>
#include <vector>

#include "Span.hpp"

int main(void) {
    {
        std::cout << "=== Basic test ===\n" << std::endl;

        Span sp(5);
        sp.addNumber(6);
        sp.addNumber(3);
        sp.addNumber(17);
        sp.addNumber(9);
        sp.addNumber(11);

        try {
            std::cout << "shortestSpan\t: " << sp.shortestSpan() << std::endl;
            std::cout << "longestSpan\t: " << sp.longestSpan() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    {
        std::cout << "\n=== Not enough numbers ===\n" << std::endl;

        Span sp(1);
        sp.addNumber(42);

        std::cout << "shortestSpan\t: ";
        try {
            std::cout << sp.shortestSpan() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }

        std::cout << "longestSpan\t: ";
        try {
            std::cout << sp.longestSpan() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    {
        std::cout << "\n=== Overflow with addNumber ===\n" << std::endl;

        Span sp(2);
        try {
            sp.addNumber(1);
            sp.addNumber(2);
            sp.addNumber(3);
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }

    {
        std::cout << "\n=== addRange (vector) ===\n" << std::endl;

        std::vector<int> values;
        for (int i = 0; i < 10; ++i)
            values.push_back(i * 3);

        Span sp(10);
        try {
            sp.addRange(values.begin(), values.end());
            std::cout << "shortestSpan\t: " << sp.shortestSpan() << std::endl;
            std::cout << "longestSpan\t: " << sp.longestSpan() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }

        std::cout << "addRange overflow\t: ";
        try {
            sp.addRange(values.begin(), values.end());
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }

    {
        std::cout << "\n=== Large random test (addRange) ===\n" << std::endl;

        const unsigned int N = 10000;
        Span               sp(N);

        std::vector<int> values;
        values.reserve(N);

        std::srand(static_cast<unsigned int>(std::time(0)));
        for (unsigned int i = 0; i < N; ++i)
            values.push_back(std::rand());

        try {
            sp.addRange(values.begin(), values.end());
            std::cout << "shortestSpan\t: " << sp.shortestSpan() << std::endl;
            std::cout << "longestSpan\t: " << sp.longestSpan() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    return 0;
}