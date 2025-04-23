#include "FragTrap.hpp"

FragTrap::FragTrap() : ClapTrap() {
    std::cout << "[FragTrap] Default constructor called" << std::endl;
    _hitPoint = 100;
    _energyPoint = 100;
    _attackDamage = 30;
}

FragTrap::FragTrap(std::string name) : ClapTrap(name) {
    std::cout << "[FragTrap] Constructor with name called" << std::endl;
    _hitPoint = 100;
    _energyPoint = 100;
    _attackDamage = 30;
}

FragTrap::FragTrap(const FragTrap& other) : ClapTrap(other) {
    std::cout << "[FragTrap] Copy constructor called" << std::endl;
}

FragTrap::~FragTrap() {
    std::cout << "[FragTrap] Destructor called" << std::endl;
}

FragTrap& FragTrap::operator=(const FragTrap& other) {
    std::cout << "[FragTrap] Copy assignment operator called" << std::endl;
    if (this != &other) {
        _name = other.getName();
        _hitPoint = other.getHitPoint();
        _energyPoint = other.getEnergyPoint();
        _attackDamage = other.getAttackDamage();
    }
    return (*this);
}

void FragTrap::highFivesGuys(void) {
	std::cout << "FragTrap " << this->getName() << " asks for a High Five." << std::endl;
}
