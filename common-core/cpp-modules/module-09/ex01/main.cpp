#include <iostream>

#include "RPN.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    RPN rpnCalculator;
    rpnCalculator.processInput(argv[1]);

    return 0;
}