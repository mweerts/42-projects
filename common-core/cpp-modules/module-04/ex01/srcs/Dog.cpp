#include "Dog.hpp"

Dog::Dog() {
    std::cout << "[DOG] Default constructor called" << std::endl;
    _type = "Dog";
    _brain = new Brain();
    if (!_brain) {
        std::cerr << "Brain memory allocation failed.";
        exit(1);
    }
}

Dog::Dog(const Dog& other) : Animal() {
    std::cout << "[DOG] Copy constructor called" << std::endl;
    *this = other;
}

Dog::~Dog() {
    delete _brain;
    std::cout << "[DOG] Destructor called" << std::endl;
}

Dog& Dog::operator=(const Dog& other) {
    std::cout << "[DOG] Copy assignment operator called" << std::endl;
    if (this != &other) {
        _type = other._type;
        this->_brain = new Brain(*other._brain);
        if (!_brain) {
            std::cerr << "Brain memory allocation failed.";
            exit(1);
        }
    }
    return (*this);
}

void Dog::makeSound(void) const {
    std::cout << "Wouaf !" << std::endl;
}

Brain* Dog::getBrain(void) {
    return (_brain);
}