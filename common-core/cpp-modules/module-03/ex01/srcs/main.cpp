#include "ScavTrap.hpp"

int main(void) {
    ScavTrap scav("Bob");

    std::cout << "NAME : " << scav.getName() << " HP : " << scav.getHitPoint()
              << " ENERGY : " << scav.getEnergyPoint()
              << " ATTACK DAMAGES : " << scav.getAttackDamage() << std::endl;

    scav.attack("John");

    std::cout << "NAME : " << scav.getName() << " HP : " << scav.getHitPoint()
              << " ENERGY : " << scav.getEnergyPoint()
              << " ATTACK DAMAGES : " << scav.getAttackDamage() << std::endl;

    scav.takeDamage(5);

    std::cout << "NAME : " << scav.getName() << " HP : " << scav.getHitPoint()
              << " ENERGY : " << scav.getEnergyPoint()
              << " ATTACK DAMAGES : " << scav.getAttackDamage() << std::endl;

    scav.beRepaired(2);

    std::cout << "NAME : " << scav.getName() << " HP : " << scav.getHitPoint()
              << " ENERGY : " << scav.getEnergyPoint()
              << " ATTACK DAMAGES : " << scav.getAttackDamage() << std::endl;

    scav.takeDamage(200);

    std::cout << "NAME : " << scav.getName() << " HP : " << scav.getHitPoint()
              << " ENERGY : " << scav.getEnergyPoint()
              << " ATTACK DAMAGES : " << scav.getAttackDamage() << std::endl;

    scav.attack("John");
    scav.beRepaired(2);

    ScavTrap scav_energy("Tony");
    for (int i = 0; i < 51; i++)
        scav_energy.attack("John");
    return (0);
}