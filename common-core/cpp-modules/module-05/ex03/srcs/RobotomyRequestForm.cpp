#include "RobotomyRequestForm.hpp"

RobotomyRequestForm::RobotomyRequestForm()
    : AForm("RobotomyRequestForm", 72, 45), _target("DEFAULT") {}

RobotomyRequestForm::RobotomyRequestForm(const std::string target)
    : AForm("RobotomyRequestForm", 72, 45), _target(target) {}

RobotomyRequestForm::RobotomyRequestForm(const RobotomyRequestForm& other)
    : AForm("RobotomyRequestForm", 72, 45) {
    *this = other;
}

RobotomyRequestForm::~RobotomyRequestForm() {}

RobotomyRequestForm& RobotomyRequestForm::operator=(
    const RobotomyRequestForm& other) {
    if (this != &other) {
        AForm::operator=(other);
    }
    return (*this);
}

void RobotomyRequestForm::doExecute(void) const {
    srand(time(0));
    if (rand() % 2)
        std::cout << this->getTarget() << " has been robotomized successfully."
                  << std::endl;
    else
        std::cout << this->getTarget() << " robotomization failed."
                  << std::endl;
}

const std::string& RobotomyRequestForm::getTarget(void) const {
    return (_target);
}
