#include "Contact.hpp"

Contact::Contact()
{
	
}

void Contact::setFirstName(std::string firstName)
{
	_firstName = firstName;
}

void Contact::setLastName(std::string lastName)
{
	_lastName = lastName;
}

void Contact::setNickName(std::string nickName)
{
	_nickName = nickName
}

void Contact::setDarkestSecret(std::string darkestSecret)
{
	_darkestSecret = darkestSecret;
}

void Contact::setPhoneNumber(std::string phoneNumber)
{
	_phoneNumber = phoneNumber;
}

std::string Contact::getFirstName(void) const
{
	return (_firstName);
}

std::string Contact::getLastName(void) const
{
	return (_lastName);
}

std::string Contact::getNickName(void) const
{
	return (_nickName);
}

std::string Contact::getDarkestSecret(void) const
{
	return (_darkestSecret);
}

std::string Contact::getPhoneNumber(void) const
{
	return (_phoneNumber);
}