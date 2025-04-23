#ifndef CLAPTRAP_HPP
#define CLAPTRAP_HPP

#include <iostream>
#include <string>

class ClapTrap {
   public:
    ClapTrap();
    ClapTrap(std::string name);
    ClapTrap(const ClapTrap &other);
    ~ClapTrap();
    ClapTrap   &operator=(const ClapTrap &other);
    void        attack(const std::string &target);
    void        takeDamage(unsigned int amount);
    void        beRepaired(unsigned int amount);
    std::string getName(void) const;
    int         getHitPoint(void) const;
    int         getEnergyPoint(void) const;
    int         getAttackDamage(void) const;

   private:
    std::string _name;
    int         _hitPoint;
    int         _energyPoint;
    int         _attackDamage;
};

#endif