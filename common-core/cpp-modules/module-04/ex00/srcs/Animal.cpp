#include "Animal.hpp"

Animal::Animal() : _type("Animal") {
    std::cout << "[ANIMAL] Default constructor called" << std::endl;
}

Animal::Animal(const Animal& other) {
    std::cout << "[ANIMAL] Copy constructor called" << std::endl;
    *this = other;
}

Animal::~Animal() {
    std::cout << "[ANIMAL] Destructor called" << std::endl;
}

Animal& Animal::operator=(const Animal& other) {
    std::cout << "[ANIMAL] Copy assignment operator called" << std::endl;
    if (this != &other) {
        _type = other._type;
    }
    return (*this);
}

void Animal::makeSound(void) const {
	std::cout << "Animal makes sound" << std::endl;
}

std::string Animal::getType(void) const {
    return (_type);
}
