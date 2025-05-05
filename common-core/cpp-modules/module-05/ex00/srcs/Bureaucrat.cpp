#include "Bureaucrat.hpp"

Bureaucrat::Bureaucrat() : _name("DEFAULT"), _grade(75) {}

Bureaucrat::Bureaucrat(std::string name, int grade) : _name(name) {
    if (grade < 1)
        throw GradeTooHighException();
    if (grade > 150)
        throw GradeTooLowException();
    _grade = grade;
}

Bureaucrat::Bureaucrat(const Bureaucrat& other) {
    *this = other;
}

Bureaucrat::~Bureaucrat() {}

Bureaucrat& Bureaucrat::operator=(const Bureaucrat& other) {
    if (this != &other) {
        _grade = other._grade;
    }
    return (*this);
}

std::string Bureaucrat::getName(void) const {
    return (_name);
}

int Bureaucrat::getGrade(void) const {
    return (_grade);
}

void Bureaucrat::incrementGrade(void) {
    if (this->getGrade() - 1 < 1)
        throw GradeTooHighException();
    _grade--;
}

void Bureaucrat::decrementGrade(void) {
    if (this->getGrade() + 1 > 150)
        throw GradeTooLowException();
    _grade++;
}

const char* Bureaucrat::GradeTooHighException::what(void) const throw() {
	return ("Grade too high !");
}

const char* Bureaucrat::GradeTooLowException::what(void) const throw() {
	return ("Grade too low !");
}

std::ostream& operator<<(std::ostream& os, const Bureaucrat& obj) {
    os << obj.getName() << ", bureaucrat grade " << obj.getGrade() << ".";
	return (os);
}
