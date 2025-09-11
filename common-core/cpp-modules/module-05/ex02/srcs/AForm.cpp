#include "AForm.hpp"

AForm::AForm()
    : _name("DEFAULT"), _signed(false), _gradeToSign(75), _gradeToExec(75) {}

AForm::AForm(std::string name, const int gradeToSign, const int gradeToExec)
    : _name(name),
      _signed(false),
      _gradeToSign(gradeToSign),
      _gradeToExec(gradeToExec) {
    if (gradeToSign < 1 || gradeToExec < 1)
        throw GradeTooHighException();
    if (gradeToSign > 150 || gradeToExec > 150)
        throw GradeTooLowException();
}

AForm::AForm(const AForm& other)
    : _name(other._name),
      _signed(other._signed),
      _gradeToSign(other._gradeToSign),
      _gradeToExec(other._gradeToExec) {
    *this = other;
}

AForm::~AForm() {}

AForm& AForm::operator=(const AForm& other) {
    if (this != &other) {
        _signed = other._signed;
    }
    return (*this);
}

const std::string& AForm::getName(void) const {
    return (_name);
}

const bool& AForm::isSigned(void) const {
    return (_signed);
}

const int& AForm::getGradeToSign(void) const {
    return (_gradeToSign);
}

const int& AForm::getGradeToExec(void) const {
    return (_gradeToExec);
}

void AForm::beSigned(Bureaucrat& bureaucrat) {
    if (bureaucrat.getGrade() > getGradeToSign())
        throw GradeTooLowException();
    if (isSigned())
        throw "Already signed !";
    _signed = true;
}

void AForm::execute(Bureaucrat const& executor) const {
    (void)executor;
    if (!this->isSigned())
        throw NotSignedException();
    else if (executor.getGrade() > this->getGradeToExec())
		throw GradeTooLowException();
    else
        this->doExecute();
}

const char* AForm::GradeTooHighException::what(void) const throw() {
    return ("Grade too high !");
}

const char* AForm::GradeTooLowException::what(void) const throw() {
    return ("Grade too low !");
}

const char* AForm::NotSignedException::what(void) const throw() {
    return ("Form not signed !");
}

std::ostream& operator<<(std::ostream& os, const AForm& obj) {
    os << obj.getName() << " form is ";
    if (obj.isSigned())
        os << "signed.";
    else
        os << "unsigned.";
    os << " The required grade to sign it is " << obj.getGradeToSign()
       << " and the required grade to execute it is " << obj.getGradeToExec()
       << "." << std::endl;
    return (os);
}