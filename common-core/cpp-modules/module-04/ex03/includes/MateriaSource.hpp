#ifndef MATERIASOURCE_HPP
#define MATERIASOURCE_HPP

#include "IMateriaSource.hpp"
#include <iostream>

class MateriaSource : public IMateriaSource {
   public:
    MateriaSource();
    MateriaSource(const MateriaSource &other);
    ~MateriaSource();
    MateriaSource &operator=(const MateriaSource &other);

    virtual void      learnMateria(AMateria *m);
    virtual AMateria *createMateria(std::string const &type);

   private:
    AMateria *_materiaSourceInventory[4];
};

#endif