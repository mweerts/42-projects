#include "PhoneBook.hpp"

int	main(int argc, char **argv)
{
	std::string input;
	PhoneBook phoneBook;

	std::cout << "Welcome to your awesome PhoneBook !\n";
	while (true)
	{
		std::cout << "Type ADD, SEARCH or EXIT :\n";
		std::getline(std::cin, input);
		if (input == "ADD")
			phoneBook.add();
		else if (input == "SEARCH")
			phoneBook.search();
		else if (input == "EXIT")
		{
			std::cout << "Bye Bye !\n";
			break;
		}
		else
			std::cout << RED BOLD "Wrong Input !\n" END;
	}
	return (0);
}