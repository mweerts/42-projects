#ifndef __PHONEBOOK_HPP__
#define __PHONEBOOK_HPP__

#include "Contact.hpp"

class PhoneBook {
   private:
    Contact _contactArr[8];
    int _currIndex;
    int _nbContact;

   public:
    PhoneBook();
    void add(Contact contact);
    void search(int index);
};

#endif