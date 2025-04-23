#ifndef FRAGTRAP_HPP
#define FRAGTRAP_HPP

#include <iostream>
#include <string>

#include "ClapTrap.hpp"

class FragTrap : virtual public ClapTrap {
   public:
    FragTrap();
    FragTrap(std::string name);
    FragTrap(const FragTrap &other);
    ~FragTrap();
    FragTrap &operator=(const FragTrap &other);
    void      highFivesGuys(void);

   protected:
    const static int _hitPointsConst = 100;
    const static int _energyPointsConst = 100;
    const static int _attackDamageConst = 30;
};

#endif