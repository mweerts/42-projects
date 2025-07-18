#include "AForm.hpp"
#include "Bureaucrat.hpp"
#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "Intern.hpp"

/**
 *  Définitions des codes de couleur ANSI pour la sortie console
 */
#define GREY "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define NC "\033[0m"

/**
 *  Point d'entrée principal du programme
 */
int main()
{
	Intern intern;
	AForm *form;

	// Test de création de formulaire avec un nom valide
	try
	{
		form = intern.makeForm("ShrubberyCreationForm", "home");
		if (form)
		{
			std::cout << GREEN << "Form created: " << form->getName() << NC << std::endl;
			delete form; 
		}
		else
			std::cout << RED << "Form creation failed." << NC << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << RED << "Exception: " << e.what() << NC << std::endl;
	}


	// Test de création de formulaire avec un autre nom valide
		try
	{
		form = intern.makeForm("PresidentialPardonForm", "123");
		if (form)
		{
			std::cout << GREEN << "Form created: " << form->getName() << NC << std::endl;
			delete form; 
		}
		else
			std::cout << RED << "Form creation failed." << NC << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << RED << "Exception: " << e.what() << NC << std::endl;
	}

	// Test de création de formulaire avec un autre nom valide
		try
	{
		form = intern.makeForm("RobotomyRequestForm", "");
		if (form)
		{
			std::cout << GREEN << "Form created: " << form->getName() << NC << std::endl;
			delete form;
		}
		else
			std::cout << RED << "Form creation failed." << NC << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << RED << "Exception: " << e.what() << NC << std::endl;
	}


	// Test de création de formulaire avec un nom invalide
	try
	{
		form = intern.makeForm("fwewef", "home");
		if (form)
		{
			std::cout << GREEN << "Form created: " << form->getName() << NC << std::endl;
			delete form;
		}
		else
			std::cout << RED << "Form creation failed." << NC << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << RED << "Exception: " << e.what() << NC << std::endl;
	}

	
	return 0;
}
