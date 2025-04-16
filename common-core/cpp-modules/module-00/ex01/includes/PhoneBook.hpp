#ifndef __PHONEBOOK_HPP__
#define __PHONEBOOK_HPP__

#include "Contact.hpp"

class PhoneBook {
   private:
    Contact _contactArr[8];
    int _currIndex;
    int _nbContact;

   public:
    PhoneBook(void);
    ~PhoneBook(void);
    void add(void);
    void search(void);
    void printMenu(void);
};

#endif