#ifndef DOG_HPP
#define DOG_HPP

#include "Animal.hpp"
#include "Brain.hpp"

class Dog : public Animal {
   public:
    Dog();
    Dog(const Dog &other);
    ~Dog();
    Dog   &operator=(const Dog &other);
    void   makeSound(void) const;
    Brain *getBrain(void);

   private:
    Brain *_brain;
};

#endif