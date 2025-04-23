#include "FragTrap.hpp"

int main(void) {
    FragTrap frag("Bob");

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

    FragTrap frag_energy("Tony");
    for (int i = 0; i < 101; i++)
        frag_energy.attack("John");
    
    frag_energy.highFivesGuys();
    return (0);
}