#include "DiamondTrap.hpp"

int main(void) {
    DiamondTrap frag("Bob");

    std::cout << "NAME : " << frag.getName() << " HP : " << frag.getHitPoint()
              << " ENERGY : " << frag.getEnergyPoint()
              << " ATTACK DAMAGES : " << frag.getAttackDamage() << std::endl;

    frag.attack("John");

    std::cout << "NAME : " << frag.getName() << " HP : " << frag.getHitPoint()
              << " ENERGY : " << frag.getEnergyPoint()
              << " ATTACK DAMAGES : " << frag.getAttackDamage() << std::endl;

    frag.takeDamage(5);

    std::cout << "NAME : " << frag.getName() << " HP : " << frag.getHitPoint()
              << " ENERGY : " << frag.getEnergyPoint()
              << " ATTACK DAMAGES : " << frag.getAttackDamage() << std::endl;

    frag.beRepaired(2);

    std::cout << "NAME : " << frag.getName() << " HP : " << frag.getHitPoint()
              << " ENERGY : " << frag.getEnergyPoint()
              << " ATTACK DAMAGES : " << frag.getAttackDamage() << std::endl;

    frag.takeDamage(200);

    std::cout << "NAME : " << frag.getName() << " HP : " << frag.getHitPoint()
              << " ENERGY : " << frag.getEnergyPoint()
              << " ATTACK DAMAGES : " << frag.getAttackDamage() << std::endl;

    frag.attack("John");
    frag.beRepaired(2);

    DiamondTrap frag_energy("Tony");
    for (int i = 0; i < 51; i++)
        frag_energy.attack("John");

    frag_energy.highFivesGuys();
    frag.whoAmI();
    return (0);
}