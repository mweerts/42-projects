#include "Cat.hpp"

Cat::Cat() {
	std::cout << "[CAT] Default constructor called" << std::endl;
	_type = "Cat";
}

Cat::Cat(const Cat& other) : Animal() {
    std::cout << "[CAT] Copy constructor called" << std::endl;
    *this = other;
}

Cat::~Cat() {
    std::cout << "[CAT] Destructor called" << std::endl;
}

Cat& Cat::operator=(const Cat& other) {
    std::cout << "[CAT] Copy assignment operator called" << std::endl;
    if (this != &other) {
        _type = other._type;
    }
    return (*this);
}

void Cat::makeSound(void) const {
	std::cout << "Meow !" << std::endl;
}