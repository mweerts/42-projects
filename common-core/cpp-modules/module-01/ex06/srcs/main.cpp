#include "Harl.hpp"

#define BOLD "\033[1m"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define END "\033[0m"

int main(int argc, char **argv) {
	Harl harl;
    if (argc != 2) {
        std::cout << RED << "error: Wrong number of Arguments." << END
                  << std::endl;
        return (0);
    }
	harl.complain((std::string)argv[1]);
	return (0);
}