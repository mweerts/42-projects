#ifndef ROBOTOMYREQUESTFORM_HPP
#define ROBOTOMYREQUESTFORM_HPP

#include <iostream>
#include <cstdlib>
#include <ctime>

#include "AForm.hpp"

class Bureaucrat;
class AForm;

class RobotomyRequestForm : public AForm {
   public:
    RobotomyRequestForm();
    RobotomyRequestForm(const std::string target);
    RobotomyRequestForm(const RobotomyRequestForm &other);
    ~RobotomyRequestForm();
    RobotomyRequestForm &operator=(const RobotomyRequestForm &other);

    void               doExecute(void) const;
    const std::string &getTarget(void) const;

   private:
    const std::string _target;
};

#endif