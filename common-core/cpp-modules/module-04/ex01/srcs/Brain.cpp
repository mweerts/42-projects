#include "Brain.hpp"

Brain::Brain() {
    std::cout << "[BRAIN] Default constructor called" << std::endl;
}

Brain::Brain(const Brain& other) {
    std::cout << "[BRAIN] Copy constructor called" << std::endl;
    *this = other;
}

Brain::~Brain() {
    std::cout << "[BRAIN] Destructor called" << std::endl;
}

Brain& Brain::operator=(const Brain& other) {
    std::cout << "[BRAIN] Copy assignment operator called" << std::endl;
    if (this != &other) {
        for (int i = 0; i < 100; i++)
            _ideas[i] = other._ideas[i];
    }
    return (*this);
}

const std::string Brain::getIdea(size_t index) const {
    if (index < 100)
        return (_ideas[index]);
    return ("Index Error");
}

void Brain::setIdea(size_t index, std::string idea) {
    if (index >= 100)
        return;
    _ideas[index] = idea;
}
