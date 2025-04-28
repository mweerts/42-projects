#include "Cure.hpp"

Cure::Cure() {
    _type = "cure";
}

Cure::Cure(const Cure& other) : AMateria(other) {}

Cure::~Cure() {}

Cure& Cure::operator=(const Cure& other) {
    if (this != &other) {
        _type = other.getType();
    }
    return (*this);
}

AMateria* Cure::clone() const {
    return (new Cure(*this));
}

void Cure::use(ICharacter& target) {
    std::cout << "* heals " << target.getName() << "’s wounds *" << std::endl;
}
