#ifndef __CONTACT_HPP__
#define __CONTACT_HPP__

#define BOLD "\033[1m"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define END "\033[0m"

#include <string>
#include <iostream>
#include <iomanip>
#include <stdlib.h>

class Contact {
   private:
    std::string _firstName;
    std::string _lastName;
    std::string _nickName;
    std::string _phoneNumber;
    std::string _darkestSecret;

   public:
    Contact();
    void setFirstName(std::string firstName);
    void setLastName(std::string lastName);
    void setNickName(std::string nickName);
    void setDarkestSecret(std::string darkestSecret);
    void setPhoneNumber(std::string phoneNumber);
    std::string getFirstName(void) const;
    std::string getLastName(void) const;
    std::string getNickName(void) const;
    std::string getDarkestSecret(void) const;
    std::string getPhoneNumber(void) const;
};

#endif