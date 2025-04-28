#include "Ice.hpp"

Ice::Ice() {
    _type = "ice";
}

Ice::Ice(const Ice& other) : AMateria(other) {
    *this = other;
}

Ice::~Ice() {}

Ice& Ice::operator=(const Ice& other) {
    if (this != &other) {
        _type = other.getType();
    }
    return (*this);
}

AMateria* Ice::clone() const {
    return (new Ice(*this));
}

void Ice::use(ICharacter& target) {
    std::cout << "* shoots an ice bolt at " << target.getName() << " *"
              << std::endl;
}
