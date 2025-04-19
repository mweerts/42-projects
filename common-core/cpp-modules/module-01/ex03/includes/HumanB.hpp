#ifndef __HUMANB_HPP__
#define __HUMANB_HPP__

#include <iostream>
#include <string>

#include "Weapon.hpp"

class HumanB {
   private:
    std::string _name;
    Weapon     *_weapon;

   public:
    HumanB(std::string name);
    ~HumanB();
    void attack(void) const;
    void setWeapon(Weapon &weapon);
};

HumanB::HumanB(std::string name) : _name(name) {}

HumanB::~HumanB() {}

void HumanB::attack(void) const {
    std::cout << _name << " attacks with their " << _weapon->getType()
              << std::endl;
}

void HumanB::setWeapon(Weapon &weapon) {
    _weapon = &weapon;
}

#endif