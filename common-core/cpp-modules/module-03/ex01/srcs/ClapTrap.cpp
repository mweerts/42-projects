#include "ClapTrap.hpp"

ClapTrap::ClapTrap()
    : _name("DEFAULT"), _hitPoint(10), _energyPoint(10), _attackDamage(0) {
        std::cout << "[ClapTrap] Default constructor called" << std::endl;
    }

ClapTrap::ClapTrap(std::string name)
    : _name(name), _hitPoint(10), _energyPoint(10), _attackDamage(0) {
    std::cout << "[ClapTrap] Constructor with name called" << std::endl;
}

ClapTrap::ClapTrap(const ClapTrap& other) {
    std::cout << "[ClapTrap] Copy constructor called" << std::endl;
    *this = other;
}

ClapTrap::~ClapTrap() {
    std::cout << "[ClapTrap] Destructor called" << std::endl;
}

ClapTrap& ClapTrap::operator=(const ClapTrap& other) {
    std::cout << "[ClapTrap] Copy assignment operator called" << std::endl;
    if (this != &other) {
        _name = other.getName();
        _hitPoint = other.getHitPoint();
        _energyPoint = other.getEnergyPoint();
        _attackDamage = other.getAttackDamage();
    }
    return (*this);
}

void ClapTrap::attack(const std::string& target) {
    if (_energyPoint > 0 && _hitPoint > 0) {
        std::cout << "ClapTrap " << this->getName() << " attacks " << target
                  << " causing " << this->getAttackDamage()
                  << " points of damage!" << std::endl;
        _energyPoint--;
    } else if (_hitPoint <= 0)
        std::cout << "ClapTrap " << this->getName()
                  << " cannot attack : He is dead." << std::endl;
    else
        std::cout << "ClapTrap " << this->getName()
                  << " cannot attack : No energy left." << std::endl;
}

void ClapTrap::takeDamage(unsigned int amount) {
    std::cout << "ClapTrap " << this->getName() << " receives " << amount
              << " points of damage!" << std::endl;
    _hitPoint -= amount;
    if (_hitPoint < 0)
        _hitPoint = 0;
}

void ClapTrap::beRepaired(unsigned int amount) {
    if (_energyPoint > 0 && _hitPoint > 0) {
        std::cout << "ClapTrap " << this->getName()
                  << " repairs himself, restoring " << amount << " HP!"
                  << std::endl;
        _energyPoint--;
        _hitPoint += amount;
    } else if (_hitPoint <= 0)
        std::cout << "ClapTrap " << this->getName()
                  << " cannot repair : He is dead." << std::endl;
    else
        std::cout << "ClapTrap " << this->getName()
                  << " cannot repair : No energy left." << std::endl;
}

std::string ClapTrap::getName(void) const {
    return (_name);
}

int ClapTrap::getHitPoint(void) const {
    return (_hitPoint);
}

int ClapTrap::getEnergyPoint(void) const {
    return (_energyPoint);
}

int ClapTrap::getAttackDamage(void) const {
    return (_attackDamage);
}
