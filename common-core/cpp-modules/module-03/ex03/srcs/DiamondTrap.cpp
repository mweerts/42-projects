#include "DiamondTrap.hpp"

DiamondTrap::DiamondTrap() : ClapTrap() {
    std::cout << "[DiamondTrap] Default constructor called" << std::endl;
    _name = "DEFAULT";
    _hitPoint = FragTrap::_hitPointsConst;
    _energyPoint = ScavTrap::_energyPointsConst;
    _attackDamage = FragTrap::_attackDamageConst;
}

DiamondTrap::DiamondTrap(std::string name) : ClapTrap(name + "_clap_name") {
    std::cout << "[DiamondTrap] Constructor with name called" << std::endl;
    _name = name;
    _hitPoint = FragTrap::_hitPointsConst;
    _energyPoint = ScavTrap::_energyPointsConst;
    _attackDamage = FragTrap::_attackDamageConst;
}

DiamondTrap::DiamondTrap(const DiamondTrap& other)
    : ClapTrap(other), ScavTrap(other), FragTrap(other) {
    std::cout << "[DiamondTrap] Copy constructor called" << std::endl;
    *this = other;
}

DiamondTrap::~DiamondTrap() {
    std::cout << "[DiamondTrap] Destructor called" << std::endl;
}

DiamondTrap& DiamondTrap::operator=(const DiamondTrap& other) {
    std::cout << "[DiamondTrap] Copy assignment operator called" << std::endl;
    if (this != &other) {
        _name = other.getName();
        _hitPoint = other.getHitPoint();
        _energyPoint = other.getEnergyPoint();
        _attackDamage = other.getAttackDamage();
    }
    return (*this);
}

void DiamondTrap::attack(const std::string& target) {
    ScavTrap::attack(target);
}

void DiamondTrap::whoAmI(void) {
    std::cout << "My name is " << _name << " and my ClapName is "
              << ClapTrap::_name << std::endl;
}
