#include "ClapTrap.hpp"

int main(void) {
    ClapTrap clapTrap("Bob");

    std::cout << "NAME : " << clapTrap.getName()
              << " HP : " << clapTrap.getHitPoint()
              << " ENERGY : " << clapTrap.getEnergyPoint()
              << " ATTACK DAMAGES : " << clapTrap.getAttackDamage()
              << std::endl;

    clapTrap.attack("John");

    std::cout << "NAME : " << clapTrap.getName()
              << " HP : " << clapTrap.getHitPoint()
              << " ENERGY : " << clapTrap.getEnergyPoint()
              << " ATTACK DAMAGES : " << clapTrap.getAttackDamage()
              << std::endl;

    clapTrap.takeDamage(5);

    std::cout << "NAME : " << clapTrap.getName()
              << " HP : " << clapTrap.getHitPoint()
              << " ENERGY : " << clapTrap.getEnergyPoint()
              << " ATTACK DAMAGES : " << clapTrap.getAttackDamage()
              << std::endl;

    clapTrap.beRepaired(2);

    std::cout << "NAME : " << clapTrap.getName()
              << " HP : " << clapTrap.getHitPoint()
              << " ENERGY : " << clapTrap.getEnergyPoint()
              << " ATTACK DAMAGES : " << clapTrap.getAttackDamage()
              << std::endl;

    clapTrap.attack("John");
    clapTrap.attack("John");
    clapTrap.attack("John");
    clapTrap.attack("John");
    clapTrap.attack("John");
    clapTrap.attack("John");
    clapTrap.attack("John");
    clapTrap.attack("John");

    std::cout << "NAME : " << clapTrap.getName()
              << " HP : " << clapTrap.getHitPoint()
              << " ENERGY : " << clapTrap.getEnergyPoint()
              << " ATTACK DAMAGES : " << clapTrap.getAttackDamage()
              << std::endl;
              
    clapTrap.attack("John");
    clapTrap.beRepaired(2);

    clapTrap.takeDamage(10);

    std::cout << "NAME : " << clapTrap.getName()
              << " HP : " << clapTrap.getHitPoint()
              << " ENERGY : " << clapTrap.getEnergyPoint()
              << " ATTACK DAMAGES : " << clapTrap.getAttackDamage()
              << std::endl;

    clapTrap.attack("John");
    clapTrap.beRepaired(2);
    return (0);
}