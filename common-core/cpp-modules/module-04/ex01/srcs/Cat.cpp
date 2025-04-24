#include "Cat.hpp"

Cat::Cat() {
    std::cout << "[CAT] Default constructor called" << std::endl;
    _type = "Cat";
    _brain = new Brain();
    if (!_brain) {
        std::cerr << "Brain memory allocation failed.";
        exit(1);
    }
}

Cat::Cat(const Cat& other) : Animal() {
    std::cout << "[CAT] Copy constructor called" << std::endl;
    *this = other;
}

Cat::~Cat() {
    delete _brain;
    std::cout << "[CAT] Destructor called" << std::endl;
}

Cat& Cat::operator=(const Cat& other) {
    std::cout << "[CAT] Copy assignment operator called" << std::endl;
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

void Cat::makeSound(void) const {
    std::cout << "Meow !" << std::endl;
}

Brain* Cat::getBrain(void) {
    return (_brain);
}
