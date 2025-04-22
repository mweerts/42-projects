#ifndef __WEAPON_HPP__
#define __WEAPON_HPP__

#include <string>

class Weapon {
   private:
    std::string _type;

   public:
    Weapon(const std::string type);
    ~Weapon();
    const std::string &getType(void);
    void               setType(const std::string type);
};

#endif