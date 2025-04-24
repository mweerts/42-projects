#include "WrongAnimal.hpp"

WrongAnimal::WrongAnimal() : _type("WrongAnimal") {
    std::cout << "[WRONGANIMAL] Default constructor called" << std::endl;
}

WrongAnimal::WrongAnimal(const WrongAnimal& other) {
    std::cout << "[WRONGANIMAL] Copy constructor called" << std::endl;
    *this = other;
}

WrongAnimal::~WrongAnimal() {
    std::cout << "[WRONGANIMAL] Destructor called" << std::endl;
}

WrongAnimal& WrongAnimal::operator=(const WrongAnimal& other) {
    std::cout << "[WRONGANIMAL] Copy assignment operator called" << std::endl;
    if (this != &other) {
        _type = other._type;
    }
    return (*this);
}

void WrongAnimal::makeSound(void) const {
    std::cout << "WrongAnimal makes sound" << std::endl;
}

std::string WrongAnimal::getType(void) const {
    return (_type);
}
