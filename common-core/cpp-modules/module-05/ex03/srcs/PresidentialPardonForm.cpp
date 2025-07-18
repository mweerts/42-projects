#include "PresidentialPardonForm.hpp"

PresidentialPardonForm::PresidentialPardonForm()
    : AForm("PresidentialPardonForm", 25, 5), _target("DEFAULT") {}

PresidentialPardonForm::PresidentialPardonForm(const std::string target)
    : AForm("PresidentialPardonForm", 25, 5), _target(target) {}

PresidentialPardonForm::PresidentialPardonForm(
    const PresidentialPardonForm& other)
    : AForm("PresidentialPardonForm", 25, 5) {
    *this = other;
}

PresidentialPardonForm::~PresidentialPardonForm() {}

PresidentialPardonForm& PresidentialPardonForm::operator=(
    const PresidentialPardonForm& other) {
    if (this != &other) {
        AForm::operator=(other);
    }
    return (*this);
}

void PresidentialPardonForm::doExecute(void) const {
    std::cout << this->getTarget() << " has been pardoned by Zaphod Beeblebrox."
              << std::endl;
}

const std::string& PresidentialPardonForm::getTarget(void) const {
    return (_target);
}
