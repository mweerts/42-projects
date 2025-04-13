#ifndef __CONTACT_HPP__
#define __CONTACT_HPP__

#include <string>
#include <iostream>

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