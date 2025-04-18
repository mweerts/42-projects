#include "Weapon.hpp"
#include <string>
#include <iostream>

class HumanA {
   private:
    std::string _name;
    Weapon     &_weapon;

   public:
    HumanA(std::string name, Weapon &weapon);
    ~HumanA();
    void attack(void) const;
};

HumanA::HumanA(std::string name, Weapon &weapon)
    : _name(name), _weapon(weapon) {}

HumanA::~HumanA() {}

void HumanA::attack(void) const {
	std::cout << _name << " attacks with their " << _weapon.getType() << std::endl;
}