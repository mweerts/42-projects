#include "PhoneBook.hpp"

int	main(int argc, char **argv)
{
	std::string input;
	PhoneBook phoneBook;

	while (true)
	{
		std::cin >> input;
		if (input == "EXIT")
		{
			std::cout << "Bye Bye !\n";
			break;
		}
	}
	return (0);
}