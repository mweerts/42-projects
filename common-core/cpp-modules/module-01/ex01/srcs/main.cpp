#include "Zombie.hpp"

static bool isNumberInRange(const char* str, long min, long max) {
    if (str == NULL || *str == '\0')
        return false;
    const char* ptr = str;
    if (*ptr == '+' || *ptr == '-')
        ptr++;
    while (*ptr) {
        if (!std::isdigit(*ptr))
            return false;
        ptr++;
    }
    char* end;
    long  num = std::strtol(str, &end, 10);
    if (*end != '\0' || num < min || num > max)
        return false;
    return true;
}

int main(int argc, char** argv) {
    Zombie* horde;
    long    input;

    if (argc != 2 || !isNumberInRange(argv[1], 1, 25)) {
        std::cout
            << RED
            << "error: Please provide a positive number of zombies. (MAX 25)"
            << END << std::endl;
        return (0);
    }
    input = std::strtol(argv[1], NULL, 10);
    horde = zombieHorde(input, "Bob");
    for (int i = 0; i < input; i++)
        horde[i].announce();
    delete[] horde;
    return (0);
}