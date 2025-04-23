#ifndef DIAMONDTRAP_HPP
#define DIAMONDTRAP_HPP

#include <iostream>
#include <string>

#include "ClapTrap.hpp"
#include "FragTrap.hpp"
#include "ScavTrap.hpp"

class DiamondTrap : public ScavTrap, public FragTrap {
   public:
    DiamondTrap();
    DiamondTrap(std::string name);
    DiamondTrap(const DiamondTrap &other);
    ~DiamondTrap();
    DiamondTrap &operator=(const DiamondTrap &other);
    void         attack(const std::string &target);
    void         whoAmI(void);

   private:
    std::string _name;
};

#endif