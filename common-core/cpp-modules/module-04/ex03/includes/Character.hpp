#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <iostream>

#include "ICharacter.hpp"

class AMateria;

class Character : public ICharacter {
   public:
    Character();
    Character(const std::string name);
    Character(const Character &other);
    ~Character();
    Character &operator=(const Character &other);

    virtual std::string const &getName() const;
    virtual void               equip(AMateria *m);
    virtual void               unequip(int idx);
    virtual void               use(int idx, ICharacter &target);

   private:
    std::string _name;
    AMateria   *_inventory[4];
};

#endif