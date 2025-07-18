#include "Intern.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"

Intern::Intern() {}

Intern::Intern(const Intern &other)
{
	*this = other;
}

Intern &Intern::operator=(const Intern &other)
{
	(void)other;
	return *this;
}

Intern::~Intern() {}

AForm *createShrubbery(std::string target) {
	return new ShrubberyCreationForm(target);
}

AForm *createRobotomy(std::string target) {
	return new RobotomyRequestForm(target);
}

AForm *createPresident(std::string target) {
	return new PresidentialPardonForm(target);
}

AForm *Intern::makeForm(const std::string formName, const std::string target)
{
	const std::string formsNames[] = {"ShrubberyCreationForm", "RobotomyRequestForm", "PresidentialPardonForm"};
	AForm *(*formsFun[])(std::string target) = {&createShrubbery, &createRobotomy, &createPresident};

	for (int i = 0; i < 3; ++i) {
		if (formName == formsNames[i])
			return formsFun[i](target);
	}
	std::cout << "Intern cannot create form: " << formName << " for target : " << target << std::endl;
	return NULL;
}
