#include "AMateria.hpp"

AMateria::AMateria() {}

AMateria::AMateria(std::string const& type): _type(type){}

AMateria::AMateria(const AMateria& other) {
	*this = other;
}

AMateria::~AMateria() {}

AMateria& AMateria::operator=(const AMateria& other) {
    if (this != &other) {
        _type = other._type;
    }
    return (*this);
}

std::string const& AMateria::getType() const {
    return (_type);
}
