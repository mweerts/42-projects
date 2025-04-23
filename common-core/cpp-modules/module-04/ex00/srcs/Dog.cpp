#include "Dog.hpp"

Dog::Dog() {
	std::cout << "[DOG] Default constructor called" << std::endl;
	_type = "Dog";
}

Dog::Dog(const Dog& other): Animal() {
    std::cout << "[DOG] Copy constructor called" << std::endl;
    *this = other;
}

Dog::~Dog() {
    std::cout << "[DOG] Destructor called" << std::endl;
}

Dog& Dog::operator=(const Dog& other) {
    std::cout << "[DOG] Copy assignment operator called" << std::endl;
    if (this != &other) {
        _type = other._type;
    }
    return (*this);
}

void Dog::makeSound(void) const {
	std::cout << "Wouaf !" << std::endl;
}