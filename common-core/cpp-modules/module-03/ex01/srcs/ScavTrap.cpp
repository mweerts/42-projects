#include "ScavTrap.hpp"

ScavTrap::ScavTrap() : ClapTrap() {
    std::cout << "[ScavTrap] Default constructor called" << std::endl;
    _hitPoint = 100;
    _energyPoint = 50;
    _attackDamage = 20;
    _gateKeeperMode = false;
}

ScavTrap::ScavTrap(std::string name) : ClapTrap(name) {
    std::cout << "[ScavTrap] Default constructor called" << std::endl;
    _hitPoint = 100;
    _energyPoint = 50;
    _attackDamage = 20;
    _gateKeeperMode = false;
}

ScavTrap::ScavTrap(const ScavTrap& other) : ClapTrap(other) {
    std::cout << "[ScavTrap] Copy constructor called" << std::endl;
}

ScavTrap::~ScavTrap() {
    std::cout << "[ScavTrap] Destructor called" << std::endl;
}

ScavTrap& ScavTrap::operator=(const ScavTrap& other) {
    std::cout << "[ScavTrap] Copy assignment operator called" << std::endl;
    if (this != &other) {
        _name = other.getName();
        _hitPoint = other.getHitPoint();
        _energyPoint = other.getEnergyPoint();
        _attackDamage = other.getAttackDamage();
        _gateKeeperMode = other.getMode();
    }
    return (*this);
}

void ScavTrap::guardGate(void) {
    if (_hitPoint > 0) {
        std::cout << "ScavTrap " << this->getName()
                  << " is now in Gate keeper mode." << std::endl;
        _gateKeeperMode = true;
    } else
        std::cout << "ScavTrap " << this->getName()
                  << " cannot change his state to Gate keeper mode: He is dead." << std::endl;
}

bool ScavTrap::getMode(void) const {
    return (_gateKeeperMode);
}

void ScavTrap::attack(const std::string& target) {
	if (_energyPoint > 0 && _hitPoint > 0) {
        std::cout << "ScavTrap " << this->getName() << " attacks " << target
                  << " causing " << this->getAttackDamage()
                  << " points of damage!" << std::endl;
        _energyPoint--;
    } else if (_hitPoint <= 0)
        std::cout << "ScavTrap " << this->getName()
                  << " cannot attack : He is dead." << std::endl;
    else
        std::cout << "ScavTrap " << this->getName()
                  << " cannot attack : No energy left." << std::endl;
}
