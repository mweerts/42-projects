#ifndef __ZOMBIE_HPP__
#define __ZOMBIE_HPP__

#define BOLD "\033[1m"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define END "\033[0m"

#include <iostream>
#include <string>
#include <stdlib.h>

class Zombie {
   private:
    std::string _name;

   public:
    Zombie();
    Zombie(std::string name);
    ~Zombie();
    void announce(void);
    void setName(std::string name);
};

Zombie* zombieHorde(int N, std::string name);

#endif