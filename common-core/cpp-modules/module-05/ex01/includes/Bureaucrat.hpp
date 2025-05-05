#ifndef BUREAUCRAT_HPP
#define BUREAUCRAT_HPP

#include <exception>
#include <iostream>
#include <string>

#include "Form.hpp"

class Form;

class Bureaucrat {
   public:
    Bureaucrat();
    Bureaucrat(std::string name, int grade);
    Bureaucrat(const Bureaucrat &other);
    ~Bureaucrat();
    Bureaucrat &operator=(const Bureaucrat &other);

    // GETTERS
    const std::string &getName(void) const;
    const int         &getGrade(void) const;

    // INCREMENT - DECREMENT
    void incrementGrade(void);
    void decrementGrade(void);

    void signForm(Form &form);

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
    int               _grade;
};

std::ostream &operator<<(std::ostream &os, const Bureaucrat &obj);

#endif