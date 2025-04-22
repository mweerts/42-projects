#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define BOLD "\033[1m"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define END "\033[0m"

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cerr << RED << "error : Wrong number of arguments" << END
                  << std::endl
                  << "Usage:\t./replace <file_name> <search> <replace>"
                  << std::endl;
        return (EXIT_FAILURE);
    }
    std::string fileName = argv[1];
    std::string       s1 = argv[2];
    std::string       s2 = argv[3];
    std::string       inputStr = "";
    std::ifstream     inputFile(fileName.c_str());
    std::ofstream     outputFile;
    std::string       str;

	if (s1.empty()) {
		std::cerr << RED << "error: The search string cannot be empty." << END << std::endl;
		return (EXIT_FAILURE);
	}
    if (!inputFile.is_open()) {
        std::cerr << RED << "error: " << fileName << ": "
                  << std::strerror(errno) << END << std::endl;
        return (EXIT_FAILURE);
    }

	std::ostringstream buffer;
	buffer << inputFile.rdbuf();
	inputStr = buffer.str();

    size_t start = inputStr.find(s1);
    while (start != std::string::npos) {
        inputStr.erase(start, s1.length());
        inputStr.insert(start, s2);
        start = inputStr.find(s1);
    }

	inputFile.close();
	fileName.append(".replace");

    outputFile.open(fileName.c_str());
	if (!outputFile.is_open()) {
        std::cerr << RED << "error: " << fileName << ": "
                  << std::strerror(errno) << END << std::endl;
        return (EXIT_FAILURE);
    }
    outputFile << inputStr;
	outputFile.close();

	return (EXIT_SUCCESS);
}