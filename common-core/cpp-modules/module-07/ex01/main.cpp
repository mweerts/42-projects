#include <iostream>
#include <string>

#include "iter.hpp"

void printIntValue(int x) {
    std::cout << "int value: " << x << std::endl;
}
void printInt(int& x) {
    std::cout << "int&: " << x << std::endl;
}
void printConstInt(const int& x) {
    std::cout << "const int&: " << x << std::endl;
}
void printString(std::string& s) {
    std::cout << "string&: " << s << std::endl;
}
void printConstString(const std::string& s) {
    std::cout << "const string&: " << s << std::endl;
}

int main() {
    int         arr[] = {1, 2, 3};
    std::string strs[] = {"a", "b", "c"};

    std::cout << "1. Avec fonction prenant T (valeur)\n";
    iter(arr, 3, printIntValue);

    std::cout << "\n2. Avec fonction prenant T& (référence)\n";
    iter(arr, 3, printInt);
    iter(strs, 3, printString);

    std::cout << "\n3. Avec fonction prenant const T& (référence const)\n";
    iter(arr, 3, printConstInt);
    iter(strs, 3, printConstString);

    return 0;
}