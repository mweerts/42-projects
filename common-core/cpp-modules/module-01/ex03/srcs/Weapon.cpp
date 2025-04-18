#include "Weapon.hpp"

Weapon::Weapon(const std::string type) : _type(type) {}

Weapon::~Weapon() {}

const std::string& Weapon::getType(void) {
	std::string &typeRef = _type;
    return (typeRef);
}

void Weapon::setType(const std::string type) {
    _type = type;
}
