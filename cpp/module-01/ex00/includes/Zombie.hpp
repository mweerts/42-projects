#ifndef __ZOMBIE_HPP__
#define __ZOMBIE_HPP__

#define BOLD "\033[1m"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define END "\033[0m"

#include <iostream>
#include <string>

class Zombie {
   private:
    std::string _name;

   public:
    Zombie(std::string name);
    ~Zombie();
    void announce(void);
};

Zombie* newZombie(std::string name);
void    randomChump(std::string name);

#endif