#include <iostream>

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cout << "wrong number of arguments" << std::endl
                  << "Usage:\t./replace <file_name> <search> <replace>"
                  << std::endl;
    }
}