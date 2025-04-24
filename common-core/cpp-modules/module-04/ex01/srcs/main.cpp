#include <iostream>

#include "Animal.hpp"
#include "Cat.hpp"
#include "Dog.hpp"

int main() {
    std::cout << "GIVEN TEST : " << std::endl << std::endl;

    const Animal* j = new Dog();
    const Animal* i = new Cat();
    delete j;  // should not create a leak
    delete i;

    Animal* animalArr[20];

    std::cout << std::endl
              << std::endl
              << "SIMPLE TEST : " << std::endl
              << std::endl;

    for (int i = 0; i < 20; i++) {
        if (i < 10) {
            animalArr[i] = new Cat();
            if (!animalArr[i]) {
                std::cerr << "Animal array memory allocation failed.";
                exit(1);
            }
        } else {
            animalArr[i] = new Dog();
            if (!animalArr[i]) {
                std::cerr << "Animal array memory allocation failed.";
                exit(1);
            }
        }
    }

    for (int i = 0; i < 20; i++) {
        std::cout << "Animal at index " << i
                  << " is : " << animalArr[i]->getType() << std::endl;
        animalArr[i]->makeSound();
    }

    for (int i = 0; i < 20; i++)
        delete animalArr[i];

    std::cout << "DEEP COPY TEST : " << std::endl << std::endl;

    Dog* a = new Dog();
    // Cat* a = new Cat();
    if (!a) {
        std::cerr << "Allocation failed.";
        exit(1);
    }

    a->getBrain()->setIdea(0, "I have to sniff it");
    a->getBrain()->setIdea(1, "I have to pee on it");
    a->getBrain()->setIdea(2, "I have to sniff it again");
    a->getBrain()->setIdea(101, "some shit");

    Dog* b = new Dog(*a);
    // Cat *b = new Cat(*a);
    if (!b) {
        std::cerr << "Allocation failed.";
        exit(1);
    }

    a->getBrain()->setIdea(3, "Am I a deep copy ?");
    b->getBrain()->setIdea(3, "Yes I'am");

    std::cout << "\033[34mTesting a\033[0m" << std::endl;
    std::cout << "The " << a->getType()
              << " a has the following ideas: " << std::endl;
    std::cout << a->getBrain()->getIdea(0) << std::endl;
    std::cout << a->getBrain()->getIdea(1) << std::endl;
    std::cout << a->getBrain()->getIdea(2) << std::endl;
    std::cout << a->getBrain()->getIdea(3) << std::endl;
    std::cout << a->getBrain()->getIdea(4) << std::endl;
    std::cout << a->getBrain()->getIdea(101) << std::endl;
    std::cout << std::endl;

    std::cout << "\033[34mDeconstructing a\033[0m" << std::endl;
    delete (a);
    std::cout << std::endl;

    std::cout << "\033[34mTesting b\033[0m" << std::endl;
    std::cout << "The " << b->getType()
              << " b has the following ideas: " << std::endl;
    std::cout << b->getBrain()->getIdea(0) << std::endl;
    std::cout << b->getBrain()->getIdea(1) << std::endl;
    std::cout << b->getBrain()->getIdea(2) << std::endl;
    std::cout << b->getBrain()->getIdea(3) << std::endl;
    std::cout << b->getBrain()->getIdea(4) << std::endl;
    std::cout << b->getBrain()->getIdea(101) << std::endl;
    std::cout << std::endl;

    std::cout << "\033[34mDeconstructing b\033[0m" << std::endl;
    delete (b);
    return 0;
}