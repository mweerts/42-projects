#include "Bureaucrat.hpp"

int main(void) {
    std::cout << "Creating Bureaucrat with grade too low :" << std::endl;
    try {
        Bureaucrat a("Bob", 151);
        std::cout << a << std::endl;
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    std::cout << "\nCreating Bureaucrat with grade too high :" << std::endl;
    try {
        Bureaucrat b("John", 0);
        std::cout << b << std::endl;
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    std::cout << "\nCreating Bureaucrat and increment :" << std::endl;
    try {
        Bureaucrat c("Lucy", 5);
        std::cout << c << std::endl;
        c.incrementGrade();
        std::cout << c << std::endl;
        c.incrementGrade();
        std::cout << c << std::endl;
        c.incrementGrade();
        std::cout << c << std::endl;
        c.incrementGrade();
        std::cout << c << std::endl;
        c.incrementGrade();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    std::cout << "\nCreating Bureaucrat and decrement :" << std::endl;
    try {
        Bureaucrat d("Mary", 146);
        std::cout << d << std::endl;
        d.decrementGrade();
        std::cout << d << std::endl;
        d.decrementGrade();
        std::cout << d << std::endl;
        d.decrementGrade();
        std::cout << d << std::endl;
        d.decrementGrade();
        std::cout << d << std::endl;
        d.decrementGrade();
        std::cout << d << std::endl;

    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    return (0);
}