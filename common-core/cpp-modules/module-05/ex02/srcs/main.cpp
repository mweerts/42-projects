#include "AForm.hpp"
#include "Bureaucrat.hpp"
#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "ShrubberyCreationForm.hpp"

#include <iostream>

/* Inclusion de la bibliothèque standard pour la gestion des fichiers */
#include <fstream>

/* Inclusion de la bibliothèque standard pour la gestion des informations de type */
#include <typeinfo>

#define ASSERT_TEST(expression, message) \
    if (expression) { std::cout << "\033[32m[TEST PASSED]\033[0m " << message << std::endl; } \
    else { std::cout << "\033[31m[TEST FAILED]\033[0m " << message << std::endl; }

/**
 *  Définitions des codes de couleur ANSI pour la sortie console
 */
#define GREY        "\033[0;30m"
#define RED         "\033[0;31m"
#define GREEN       "\033[0;32m"
#define YELLOW      "\033[0;33m"
#define BLUE        "\033[0;34m"
#define MAGENTA     "\033[0;35m"
#define CYAN        "\033[0;36m"
#define NC          "\033[0m"

/**
 *  Point d'entrée principal du programme
 */
int main()
{

	Bureaucrat *original = NULL;
	Bureaucrat *copy = NULL;

	/**
	* TEST CREATION OF BUREAUCRAT WITH VALID GRADE
	*/
	std::cout << CYAN << "TEST CREATION OF BUREAUCRAT WITH VALID GRADE:" << NC  << std::endl;
	try
	{
		original = new Bureaucrat("John Doe", 75);
		std::cout << *original;
		ASSERT_TEST(original->getGrade() == 75, "Grade 75.");
		delete original;
		original = NULL;
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
		ASSERT_TEST(false, "Should not throw an exception here.");
	}

	/**
	* TEST CREATION OF BUREAUCRAT WITH TOO HIGH GRADE
	*/
	std::cout << CYAN << "\nTEST CREATION OF BUREAUCRAT WITH TOO HIGH GRADE:" << NC << std::endl;
	try
	{
		original = new Bureaucrat("Jane Doe", 0);
		std::cout << *original;
		delete original;
		original = NULL;
		ASSERT_TEST(false, "Should have thrown GradeTooHighException.");
	}
	catch (Bureaucrat::GradeTooHighException& e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
		ASSERT_TEST(true, "Correctly threw GradeTooHighException.");
	}
	catch (...)
	{
		ASSERT_TEST(false, "Wrong exception type caught.");
	}

	/**
	* TEST CREATION OF BUREAUCRAT WITH TOO LOW GRADE
	*/
	std::cout << CYAN << "\nTEST CREATION OF BUREAUCRAT WITH TOO LOW GRADE:" << NC << std::endl;
	try
	{
		original = new Bureaucrat("Jim Doe", 151);
		std::cout << *original;
		delete original;
		original = NULL;
		ASSERT_TEST(false, "Should have thrown GradeTooLowException.");
	}
	catch (Bureaucrat::GradeTooLowException& e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
		ASSERT_TEST(true, "Correctly threw GradeTooLowException.");
	}
	catch (...)
	{
		ASSERT_TEST(false, "Wrong exception type caught.");
	}

	/**
	* TEST INCREMENTING AND DECREMENTING BUREAUCRAT'S GRADE
	*/
	std::cout << CYAN << "\nTEST INCREMENTING AND DECREMENTING BUREAUCRAT'S GRADE:" << NC << std::endl;
	try
	{
		original = new Bureaucrat("Julia Doe", 10);
		std::cout << *original;
		original->incrementGrade();
		std::cout << BLUE << "\nAfter incrementing bureaucrat :" << NC << std::endl;
		std::cout << *original;
		ASSERT_TEST(original->getGrade() == 9, "Grade incremented to 9.");

		original->decrementGrade();
		std::cout << BLUE << "\nAfter decrementing bureaucrat :" << NC << std::endl;

		std::cout << *original;
		ASSERT_TEST(original->getGrade() == 10, "Grade decremented to 10.");

		delete original;
		original = NULL;
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
		ASSERT_TEST(false, "No exception should be thrown here.");
	}

	/**
	* TEST COPY CONSTRUCTOR
	*/
	std::cout << CYAN << "\nTEST COPY CONSTRUCTOR:" << NC << std::endl;
	try
	{		
		original = new Bureaucrat("Bureaucrat", 50);
		std::cout << "Original " << *original;

		copy = new Bureaucrat(*original);
		std::cout << "Copy " << *copy;

		ASSERT_TEST(copy->getName() == original->getName(), "Copy have the same name.");
		ASSERT_TEST(copy->getGrade() == original->getGrade(), "Copy have the same grade.");

		copy->incrementGrade();
		std::cout << BLUE << "\nAfter incrementing copy:\n" << NC;
		std::cout << "Original " << *original;
		std::cout << "Copy " << *copy;
		ASSERT_TEST(original->getGrade() == 50, "Grade of original unchanged.");
		ASSERT_TEST(copy->getGrade() == 49, "Grade of copy incremented.");

		delete original;
		delete copy;
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
		ASSERT_TEST(false, "No exception should be thrown here.");
	}

	/**
	* TEST ASSIGNMENT OPERATOR
	*/
	std::cout << CYAN << "\nTEST ASSIGNMENT OPERATOR:" << NC << std::endl;
	try
	{
		original = new Bureaucrat("First", 75);
		copy = new Bureaucrat("Second", 100);
		std::cout <<  BLUE << "Before assignment:" << NC << std::endl << *original << *copy << std::endl;

		/**
		* Using assignment operator
		*/
		*copy = *original;
		std::cout << BLUE << "After assignment:" << NC << std::endl;
		std::cout << *original << *copy;
		ASSERT_TEST(copy->getName() == "Second", "Name of copy unchanged.");
		ASSERT_TEST(copy->getGrade() == 75, "Grade of copy updated to 75.");

		copy->incrementGrade();
		std::cout << BLUE << "\nAfter incrementing Second Bureaucrat :" << NC << std::endl;
		std::cout << *original << *copy;
		ASSERT_TEST(original->getGrade() == 75, "Grade of original unchanged.");
		ASSERT_TEST(copy->getGrade() == 74, "Grade of copy incremented.");

		delete original;
		delete copy;
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
		ASSERT_TEST(false, "No exception should be thrown here.");
	}

    /**
    * TEST BUREAUCRAT CREATION AND FORM EXECUTION
    */
    std::cout << std::endl << CYAN << "TEST BUREAUCRAT CREATION AND FORM EXECUTION:" << NC << std::endl;
    try
	{
        Bureaucrat bob("Bob", 1);
        ShrubberyCreationForm shrub("home");
        RobotomyRequestForm robot("Alice");
        PresidentialPardonForm pardon("Charlie");

        bob.signForm(shrub);
        bob.executeForm(shrub);
        ASSERT_TEST(shrub.isSigned(), "ShrubberyCreationForm signed.");
        ASSERT_TEST(shrub.isSigned() && shrub.getGradeToExec() >= bob.getGrade(), "ShrubberyCreationForm executed.");

        bob.signForm(robot);
        bob.executeForm(robot);
        ASSERT_TEST(robot.isSigned(), "RobotomyRequestForm signed.");
        ASSERT_TEST(robot.isSigned() && robot.getGradeToExec() >= bob.getGrade(), "RobotomyRequestForm executed.");

        bob.signForm(pardon);
        bob.executeForm(pardon);
        ASSERT_TEST(pardon.isSigned(), "PresidentialPardonForm signed.");
        ASSERT_TEST(pardon.isSigned() && pardon.getGradeToExec() >= bob.getGrade(), "PresidentialPardonForm executed.");
    }
	catch (std::exception &e)
	{
        std::cerr << "Exception: " << e.what() << std::endl;
        ASSERT_TEST(false, "Exception should not be thrown.");
    }

    /**
    * TEST GETTERS IN AForm
    */
    std::cout << CYAN << "\nTEST GETTERS IN AForm:" << NC << std::endl;
    try
	{
        ShrubberyCreationForm shrub("test");
        ASSERT_TEST(shrub.getName() == "ShrubberyCreationForm", "getName() works correctly.");
        ASSERT_TEST(!shrub.isSigned(), "isSigned() works correctly.");
        ASSERT_TEST(shrub.getGradeToSign() == 145, "getGradeToSign() works correctly.");
        ASSERT_TEST(shrub.getGradeToExec() == 137, "getGradeToExec() works correctly.");
    }
	catch (std::exception &e)
	{
        ASSERT_TEST(false, "Exception should not be thrown during getters tests.");
    }

    /**
    * TEST AForm CLASS NAME
    */
    std::cout << CYAN << "\nTEST AForm CLASS NAME:" << NC << std::endl;
    ASSERT_TEST(std::string(typeid(AForm).name()) == "5AForm", "AForm class name is correct.");

    /**
    * TEST ShrubberyCreationForm REQUIREMENTS
    */
    std::cout << CYAN << "\nTEST ShrubberyCreationForm REQUIREMENTS:" << NC << std::endl;
    try
	{
        ShrubberyCreationForm shrub("home");
        ASSERT_TEST(shrub.getGradeToSign() == 145, "ShrubberyCreationForm sign grade is 145.");
        ASSERT_TEST(shrub.getGradeToExec() == 137, "ShrubberyCreationForm execute grade is 137.");
    }
	catch (std::exception &e)
	{
        ASSERT_TEST(false, "ShrubberyCreationForm constructor threw an exception.");
    }

    /**
    * TEST ShrubberyCreationForm FILE CREATION
    */
    std::cout << CYAN << "\nTEST ShrubberyCreationForm:" << NC << std::endl;
    try
	{
        Bureaucrat bob("Bob", 1);
        ShrubberyCreationForm shrub("test_target");
        bob.signForm(shrub);

        /**
		 * Exécuter le formulaire avec executeForm
		*/
        bob.executeForm(shrub);
        std::ifstream infile("test_target_shrubbery");
        ASSERT_TEST(infile.good(), "Shrubbery file created using executeForm.");
        infile.close();
        std::remove("test_target_shrubbery");

        /**
		 * Exécuter le formulaire avec execute
		*/
        shrub.execute(bob);
        infile.open("test_target_shrubbery");
        ASSERT_TEST(infile.good(), "Shrubbery file created using execute.");
        infile.close();
        std::remove("test_target_shrubbery");
    }
	catch (std::exception &e)
	{
        ASSERT_TEST(false, "Exception should not be thrown during Shrubbery creation/execution.");
    }

    /**
    * TEST RobotomyRequestForm REQUIREMENTS
    */
    std::cout << CYAN << "\nTEST RobotomyRequestForm REQUIREMENTS:" << NC << std::endl;
    try
	{
        RobotomyRequestForm robot("Alice");
        ASSERT_TEST(robot.getGradeToSign() == 72, "RobotomyRequestForm sign grade is 72.");
        ASSERT_TEST(robot.getGradeToExec() == 45, "RobotomyRequestForm execute grade is 45.");
    }
	catch (std::exception &e)
	{
        ASSERT_TEST(false, "RobotomyRequestForm constructor threw an exception.");
    }

    /**
    * TEST PresidentialPardonForm REQUIREMENTS
    */
    std::cout << CYAN << "\nTEST PresidentialPardonForm REQUIREMENTS:" << NC << std::endl;
    try
	{
        PresidentialPardonForm pardon("Charlie");
        ASSERT_TEST(pardon.getGradeToSign() == 25, "PresidentialPardonForm sign grade is 25.");
        ASSERT_TEST(pardon.getGradeToExec() == 5, "PresidentialPardonForm execute grade is 5.");
    }
	catch (std::exception &e)
	{
        ASSERT_TEST(false, "PresidentialPardonForm constructor threw an exception.");
    }

   // std::remove("home_shrubbery");
    // Note: Le test suivant est là pour illustrer qu'AForm est abstraite.
    // Cette partie du code ne peut pas être exécutée car AForm ne peut pas être instanciée directement.
    // /*
    // std::cout << CYAN << "\nTEST AForm CANNOT BE INSTANTIATED:" << NC << std::endl;
    // try {
    //     // Cette ligne devrait échouer car AForm est une classe abstraite.
    //     AForm *form = new AForm("TestForm", 50, 25);
    //     ASSERT_TEST(false, "AForm should not be instantiable directly.");
    // } catch (std::exception& e) {
    //     ASSERT_TEST(true, "Correctly caught exception for AForm instantiation.");
    // }
    // */
    return 0;
}
