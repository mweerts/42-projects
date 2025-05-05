#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

void test(int num) {
    if (num < 0)
        throw std::out_of_range("Number is out of range");
    std::cout << "3\n";
}

int main(void) {
    std::string input;
    int         number;

    try {
        int* myarray = new int[10000000000];
        std::cout << "1\n";
    } catch (std::exception& e) {
        std::cout << "ERROR : " << e.what() << std::endl;
    }
    std::cout << "2\n";
    try {
        test(-10);
    } catch (std::exception& e) {
        std::cout << "ERROR : " << e.what() << std::endl;
    }
}