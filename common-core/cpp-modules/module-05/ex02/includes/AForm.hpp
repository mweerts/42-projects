#ifndef AFORM_HPP
#define AFORM_HPP

#include <iostream>
#include <string>

#include "Bureaucrat.hpp"

class Bureaucrat;

class AForm {
   public:
    AForm();
    AForm(std::string name, const int gradeToSign, const int gradeToExec);
    AForm(const AForm &other);
    virtual ~AForm();
    AForm &operator=(const AForm &other);

    // GETTERS

    const std::string &getName(void) const;
    const bool        &isSigned(void) const;
    const int         &getGradeToSign(void) const;
    const int         &getGradeToExec(void) const;

    void beSigned(Bureaucrat &bureaucrat);
    void execute(Bureaucrat const &executor) const;

    class GradeTooHighException : public std::exception {
       public:
        virtual const char *what() const throw();
    };

    class GradeTooLowException : public std::exception {
       public:
        virtual const char *what() const throw();
    };

    class NotSignedException : public std::exception {
       public:
        virtual const char *what() const throw();
    };

   protected:
    virtual void doExecute(void) const = 0;

   private:
    const std::string _name;
    bool              _signed;
    const int         _gradeToSign;
    const int         _gradeToExec;
};

std::ostream &operator<<(std::ostream &os, const AForm &obj);

#endif