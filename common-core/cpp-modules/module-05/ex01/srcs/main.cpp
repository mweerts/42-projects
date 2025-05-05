#include "Bureaucrat.hpp"

int main(void) {

    std::cout << "Creating Formular with grade too low :" << std::endl;
    try {
        Form       formA("Formulaire 1", 151, 50);
        std::cout << formA << std::endl;
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    std::cout << "\nSign formular 2 times :" << std::endl;
    try {
        Bureaucrat a("Bob", 30);
        Form       formA("Formulaire 1", 50, 50);
        std::cout << a << std::endl;
        std::cout << formA;
        a.signForm(formA);
        std::cout << formA;
        a.signForm(formA);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}