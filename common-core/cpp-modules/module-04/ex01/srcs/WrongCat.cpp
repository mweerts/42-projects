#include "WrongCat.hpp"

WrongCat::WrongCat() {
    std::cout << "[WRONGCAT] Default constructor called" << std::endl;
    _type = "WrongCat";
}

WrongCat::WrongCat(const WrongCat& other) : WrongAnimal() {
    std::cout << "[WRONGCAT] Copy constructor called" << std::endl;
    *this = other;
}

WrongCat::~WrongCat() {
    std::cout << "[WRONGCAT] Destructor called" << std::endl;
}

WrongCat& WrongCat::operator=(const WrongCat& other) {
    std::cout << "[WRONGCAT] Copy assignment operator called" << std::endl;
    if (this != &other) {
        _type = other._type;
    }
    return (*this);
}

void WrongCat::makeSound(void) const {
    std::cout << "Meow !" << std::endl;
}