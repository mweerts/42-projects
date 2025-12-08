#include <iostream>
#include <string>

#include "Array.hpp"

int main() {
    std::cout << "1. Constructeur par défaut" << std::endl;
    Array<int> empty;
    std::cout << "Taille du tableau vide: " << empty.size() << std::endl;

    std::cout << "\n2. Constructeur avec taille" << std::endl;
    Array<int> intArray(5);
    std::cout << "Taille du tableau d'int: " << intArray.size() << std::endl;
    for (unsigned int i = 0; i < intArray.size(); i++) {
        intArray[i] = i * 10;
        std::cout << "intArray[" << i << "] = " << intArray[i] << std::endl;
    }

    std::cout << "\n3. Constructeur par copie" << std::endl;
    Array<int> copyArray(intArray);
    std::cout << "Taille de la copie: " << copyArray.size() << std::endl;
    std::cout << "Modification de la copie..." << std::endl;
    copyArray[0] = 999;
    std::cout << "Original intArray[0] = " << intArray[0] << std::endl;
    std::cout << "Copie copyArray[0] = " << copyArray[0] << std::endl;

    std::cout << "\n4. Opérateur d'assignation" << std::endl;
    Array<int> assignArray;
    assignArray = intArray;
    std::cout << "Taille après assignation: " << assignArray.size()
              << std::endl;
    assignArray[1] = 777;
    std::cout << "Original intArray[1] = " << intArray[1] << std::endl;
    std::cout << "Assigné assignArray[1] = " << assignArray[1] << std::endl;

    std::cout << "\n5. Accès hors limites" << std::endl;
    try {
        std::cout << "Tentative d'accès à intArray[10]..." << std::endl;
        std::cout << intArray[10] << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Exception attrapée: " << e.what() << std::endl;
    }

    std::cout << "\n6. Tableau de strings" << std::endl;
    Array<std::string> strArray(3);
    strArray[0] = "Hello";
    strArray[1] = "World";
    strArray[2] = "!";
    for (unsigned int i = 0; i < strArray.size(); i++) {
        std::cout << "strArray[" << i << "] = " << strArray[i] << std::endl;
    }

    std::cout << "\n7. Tableau de doubles" << std::endl;
    Array<double> dblArray(4);
    for (unsigned int i = 0; i < dblArray.size(); i++) {
        dblArray[i] = i * 3.14;
        std::cout << "dblArray[" << i << "] = " << dblArray[i] << std::endl;
    }

    std::cout << "\n8. operator[] const" << std::endl;
    const Array<int> constArray(intArray);
    std::cout << "constArray[0] = " << constArray[0] << std::endl;

    return 0;
}