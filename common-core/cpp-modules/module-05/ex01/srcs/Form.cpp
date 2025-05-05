#include "Form.hpp"

Form::Form()
    : _name("DEFAULT"), _signed(false), _gradeToSign(75), _gradeToExec(75) {}

Form::Form(std::string name, const int gradeToSign, const int gradeToExec)
    : _name(name),
      _signed(false),
      _gradeToSign(gradeToSign),
      _gradeToExec(gradeToExec) {
    if (gradeToSign < 1 || gradeToExec < 1)
        throw GradeTooHighException();
    if (gradeToSign > 150 || gradeToExec > 150)
        throw GradeTooLowException();
}

Form::Form(const Form& other)
    : _name(other._name),
      _signed(other._signed),
      _gradeToSign(other._gradeToSign),
      _gradeToExec(other._gradeToExec) {
    *this = other;
}

Form::~Form() {}

Form& Form::operator=(const Form& other) {
    if (this != &other) {
        _signed = other._signed;
    }
    return (*this);
}

const std::string& Form::getName(void) const {
    return (_name);
}

const bool& Form::isSigned(void) const {
    return (_signed);
}

const int& Form::getGradeToSign(void) const {
    return (_gradeToSign);
}

const int& Form::getGradeToExec(void) const {
    return (_gradeToExec);
}

void Form::beSigned(Bureaucrat& bureaucrat) {
    if (bureaucrat.getGrade() > getGradeToSign())
        throw GradeTooLowException();
    if (isSigned())
        throw "Already signed !";
    _signed = true;
}

const char* Form::GradeTooHighException::what(void) const throw() {
    return ("Grade too high !");
}

const char* Form::GradeTooLowException::what(void) const throw() {
    return ("Grade too low !");
}

std::ostream& operator<<(std::ostream& os, const Form& obj) {
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