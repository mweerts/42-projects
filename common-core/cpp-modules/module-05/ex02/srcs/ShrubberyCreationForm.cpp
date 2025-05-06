#include "ShrubberyCreationForm.hpp"

ShrubberyCreationForm::ShrubberyCreationForm()
    : AForm("ShrubberyCreationForm", 145, 137), _target("DEFAULT") {}

ShrubberyCreationForm::ShrubberyCreationForm(const std::string target)
    : AForm("ShrubberyCreationForm", 145, 137), _target(target) {}

ShrubberyCreationForm::ShrubberyCreationForm(const ShrubberyCreationForm& other)
    : AForm("ShrubberyCreationForm", 145, 137) {
    *this = other;
}

ShrubberyCreationForm::~ShrubberyCreationForm() {}

ShrubberyCreationForm& ShrubberyCreationForm::operator=(
    const ShrubberyCreationForm& other) {
    if (this != &other) {
        AForm::operator=(other);
    }
    return (*this);
}

void ShrubberyCreationForm::doExecute(void) const {
    std::ofstream outputFile;

    outputFile.open((this->getTarget() + "_shrubbery").c_str());
    if (!outputFile.is_open()) {
        std::cout << "Cannot open " << this->getTarget() + "_shrubbery" << std::endl;
		return ;
    }
	outputFile << TREE;
}

const std::string& ShrubberyCreationForm::getTarget(void) const {
    return (_target);
}
