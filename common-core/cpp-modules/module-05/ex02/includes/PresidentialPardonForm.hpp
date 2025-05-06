#ifndef PRESIDENTIALPARDONFORM_HPP
#define PRESIDENTIALPARDONFORM_HPP

#include <iostream>

#include "AForm.hpp"

class Bureaucrat;
class AForm;

class PresidentialPardonForm : public AForm {
   public:
    PresidentialPardonForm();
    PresidentialPardonForm(const std::string target);
    PresidentialPardonForm(const PresidentialPardonForm &other);
    ~PresidentialPardonForm();
    PresidentialPardonForm &operator=(const PresidentialPardonForm &other);

    void               doExecute(void) const;
    const std::string &getTarget(void) const;

   private:
    const std::string _target;
};

#endif