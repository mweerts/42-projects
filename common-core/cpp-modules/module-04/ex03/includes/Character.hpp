#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>

#include "ICharacter.hpp"

class AMateria;

class Character : public ICharacter {
   public:
    Character();
    Character(const Character &other);
    ~Character();
    Character &operator=(const Character &other);

   private:
    std::string _name;
    AMateria   *_inventory[4];
};

#endif