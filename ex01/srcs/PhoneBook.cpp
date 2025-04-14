#include "PhoneBook.hpp"

PhoneBook::PhoneBook(): _currIndex(0), _nbContact(0)
{

}

static bool strIsNum(std::string str)
{
	if (str.length() == 0)
		return (false);
	for (int i = 0; i < str.length(); i++)
		if (!isdigit(str[i]))
			return (false);
	return (true);
}

void PhoneBook::add()
{
	std::string	firstName;
	std::string	lastName;
	std::string	nickName;
	std::string	phoneNumber;
	std::string	darkestSecret;

	std::cout << "First Name :\n";
	std::getline(std::cin, firstName);
	while (firstName.empty())
	{
		std::cout << RED BOLD "First Name Cannot Be Empty.\n" END;
		std::cout << "First Name :\n";
		getline(std::cin, firstName);
	}

	std::cout << "Last Name :\n";
	getline(std::cin, lastName);
	while (lastName.empty())
	{
		std::cout << RED BOLD "Last Name Cannot Be Empty.\n" END;
		std::cout << "Last Name :\n";
		getline(std::cin, lastName);
	}

	std::cout << "Nickname :\n";
	getline(std::cin, nickName);
	std::cout << "Phone Number :\n";
	getline(std::cin, phoneNumber);
	while (!strIsNum(phoneNumber))
	{
		std::cout << RED BOLD "Phone Number Has To Be A Valid Number.\n" END;
		std::cout << "Phone Number :\n";
		getline(std::cin, phoneNumber);
	}
	std::cout << "Darkest Secret :\n";
	getline(std::cin, darkestSecret);

	_contactArr[_currIndex].setFirstName(firstName);
	_contactArr[_currIndex].setLastName(lastName);
	_contactArr[_currIndex].setNickName(nickName);
	_contactArr[_currIndex].setPhoneNumber(phoneNumber);
	_contactArr[_currIndex].setDarkestSecret(darkestSecret);

	_currIndex = (_currIndex + 1) % 8;
	if (_nbContact < 8)
		_nbContact++;
}

void PhoneBook::search()
{
	std::cout << std::setw(10) << "INDEX" << "|";
	std::cout << std::setw(10) << "FIRST NAME" << "|";
	std::cout << std::setw(10) << "LAST NAME" << "|";
	std::cout << std::setw(10) << "NICKNAME";
	std::cout << "\n";
	for (int i = 0; i < _nbContact; i++)
	{
		std::cout << std::setw(10) << i << "|";
		if (_contactArr[i].getFirstName().length() > 10)
			std::cout << std::setw(10) << _contactArr[i].getFirstName().substr(0, 9) + "."  << "|";
		else
			std::cout << std::setw(10) << _contactArr[i].getFirstName()  << "|";
		if (_contactArr[i].getLastName().length() > 10)
			std::cout << std::setw(10) << _contactArr[i].getLastName().substr(0, 9) + "."  << "|";
		else
			std::cout << std::setw(10) << _contactArr[i].getLastName()  << "|";
		if (_contactArr[i].getNickName().length() > 10)
			std::cout << std::setw(10) << _contactArr[i].getNickName().substr(0, 9) + "."  << "|";
		else
			std::cout << std::setw(10) << _contactArr[i].getNickName()  << "|";
		std::cout << "\n";
	}
	std::cout << "\n";
}
                            