#include "Character.hpp"

#include "AMateria.hpp"

Character::Character() : _name("DEFAULT") {
    for (int i = 0; i < 4; i++)
        _inventory[i] = NULL;
}

Character::Character(const std::string name) : _name(name) {
    for (int i = 0; i < 4; i++)
        _inventory[i] = NULL;
}

Character::Character(const Character& other) {
    *this = other;
}

Character::~Character() {
	for (int i = 0; i < 4; i++)
		if (_inventory[i])
			delete _inventory[i];
}

Character& Character::operator=(const Character& other) {
    if (this != &other) {
        _name = other.getName();
        for (int i = 0; i < 4; i++) {
            if (other._inventory[i])
                _inventory[i] = other._inventory[i]->clone();
            else
                _inventory[i] = NULL;
        }
    }
    return (*this);
}

std::string const& Character::getName() const {
    return (_name);
}

void Character::equip(AMateria* m) {
    if (m)
    {
        for (int i = 0; i < 4; i++)
        {
            if (_inventory[i] == NULL)
            {
                _inventory[i] = m;
                return ;
            }
        }
        std::cout << "Cannot equip Materia : " << _name << "'s inventory is full." << std::endl;
        return ;
    }
    std::cout << "Cannot equip a non-existant materia." << std::endl;
}

void Character::unequip(int idx) {
    if (idx < 0 || idx > 3)
    {
        std::cout << "Invalid index." << std::endl;
        return ;
    }
    if ( _inventory[idx] == NULL)
    {
        std::cout << "Cannot unequip " << getName() <<"'s materia at index " << idx << ": the slot is empty." << std::endl;
        return ;
    }
    else
        _inventory[idx] = NULL;
}

void Character::use(int idx, ICharacter& target) {
    if (idx < 0 || idx > 3)
    {
        std::cout << "Invalid index." << std::endl;
        return ;
    }
    if (!_inventory[idx])
    {
        std::cout << "Cannot use " << getName() <<"'s materia at index " << idx << ": the slot is empty." << std::endl;
        return ;
    }
    if (_inventory[idx])
    _inventory[idx]->use(target);
}
