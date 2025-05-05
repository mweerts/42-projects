#ifndef FORM_HPP
#define FORM_HPP

#include <iostream>
#include <string>

#include "Bureaucrat.hpp"

class Bureaucrat;

class Form {
   public:
    Form();
    Form(std::string name, const int gradeToSign, const int gradeToExec);
    Form(const Form &other);
    ~Form();
    Form &operator=(const Form &other);

    // GETTERS

    const std::string &getName(void) const;
    const bool        &isSigned(void) const;
    const int         &getGradeToSign(void) const;
    const int         &getGradeToExec(void) const;

    void beSigned(Bureaucrat &bureaucrat);

    class GradeTooHighException : public std::exception {
       public:
        virtual const char *what() const throw();
    };

    class GradeTooLowException : public std::exception {
       public:
        virtual const char *what() const throw();
    };

   private:
    const std::string _name;
    bool              _signed;
    const int         _gradeToSign;
    const int         _gradeToExec;
};

std::ostream &operator<<(std::ostream &os, const Form &obj);

#endif