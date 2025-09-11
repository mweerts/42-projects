#include <iostream>

#include "AForm.hpp"
#include "Bureaucrat.hpp"
#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "ShrubberyCreationForm.hpp"

int main(int argc, char** argv) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <grade>" << std::endl;
        return 1;
    }
    int grade = std::atoi(argv[1]);
    if (grade < 1 || grade > 150) {
        std::cerr << "Grade must be between 1 and 150." << std::endl;
        return 1;
    }
    std::cout << "Signing and executing forms with a score " << grade << " Bureaucrat :\n" << std::endl;

    try {
        Bureaucrat a("Bob", grade);
        std::cout << a << std::endl << std::endl;

        ShrubberyCreationForm shrubberyForm("home");
        RobotomyRequestForm   robotomyForm("Alice");
        PresidentialPardonForm pardonForm("Charlie");

        std::cout << shrubberyForm << std::endl;
        std::cout << robotomyForm << std::endl;
        std::cout << pardonForm << std::endl;

        a.signForm(shrubberyForm);
        a.signForm(robotomyForm);
        a.signForm(pardonForm);

        std::cout << std::endl;

        std::cout << shrubberyForm << std::endl;
        std::cout << robotomyForm << std::endl;
        std::cout << pardonForm << std::endl;

        a.executeForm(shrubberyForm);
        a.executeForm(robotomyForm);
        a.executeForm(pardonForm);
        std::cout << std::endl;

    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
