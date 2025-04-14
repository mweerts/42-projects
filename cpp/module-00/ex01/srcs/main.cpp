#include "PhoneBook.hpp"

int	main()
{
	std::string input;
	PhoneBook phoneBook;

	std::cout << "Welcome to your awesome PhoneBook !\n\n";
	while (true)
	{
		std::cout << "Type ADD, SEARCH or EXIT :\n";
		std::getline(std::cin, input);
		if (input == "ADD")
		{
			std::cout << "\n";
			phoneBook.add();
		}
		else if (input == "SEARCH")
		{
			std::cout << "\n";
			phoneBook.search();
		}
		else if (input == "EXIT")
		{
			std::cout << "Bye Bye !\n";
			break;
		}
		else
			std::cout << RED BOLD "Wrong Input !\n" END;
		std::cout << std::endl;
	}
	return (0);
}