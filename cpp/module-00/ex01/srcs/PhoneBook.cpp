#include "PhoneBook.hpp"

PhoneBook::PhoneBook(): _currIndex(0), _nbContact(0)
{

}

void PhoneBook::add(Contact contact)
{
	_contactArr[_currIndex] = contact;
	_currIndex = (_currIndex + 1) % 8;
	if (_nbContact < 8)
		_nbContact++;
}

void PhoneBook::search(int index)
{
	if (index < 0 || index >= _nbContact)
	{
		std::cout << "Wrong index !";
		return ;
	}
	std::cout << "Index ok";
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                          