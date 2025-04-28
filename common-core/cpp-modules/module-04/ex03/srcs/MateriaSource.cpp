#include "MateriaSource.hpp"

MateriaSource::MateriaSource() {
    for (int i = 0; i < 4; i++)
        _materiaSourceInventory[i] = NULL;
}

MateriaSource::MateriaSource(const MateriaSource& other) {
    for (int i = 0; i < 4; i++) {
        if (_materiaSourceInventory[i])
            delete _materiaSourceInventory[i];
        if (other._materiaSourceInventory[i])
            _materiaSourceInventory[i] = other._materiaSourceInventory[i];
        else
            _materiaSourceInventory[i] = NULL;
    }
}

MateriaSource::~MateriaSource() {
    for (int i = 0; i < 4; i++)
        if (_materiaSourceInventory[i])
            delete _materiaSourceInventory[i];
}

MateriaSource& MateriaSource::operator=(const MateriaSource& other) {
    (void)other;
    return (*this);
}

void MateriaSource::learnMateria(AMateria* m) {
    if (m) {
        for (int i = 0; i < 4; i++) {
            if (_materiaSourceInventory[i] == NULL) {
                _materiaSourceInventory[i] = m;
                return;
            }
        }
        std::cout << "No space left in MateriaSource inventory." << std::endl;
    }
}

AMateria* MateriaSource::createMateria(std::string const& type) {
    for (int i = 0; i < 4; i++) {
        if (_materiaSourceInventory[i] &&
            type == _materiaSourceInventory[i]->getType())
            return (_materiaSourceInventory[i]->clone());
    }
    std::cout << "Cannot create Materia " << type << "." << std::endl;
    return (NULL);
}
