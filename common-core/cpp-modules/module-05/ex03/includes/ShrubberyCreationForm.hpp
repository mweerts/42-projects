#ifndef SHRUBBERYCREATIONFORM_HPP
#define SHRUBBERYCREATIONFORM_HPP

#define TREE                                                              \
    "                                              .         ;  \n"       \
    "                 .              .              ;%     ;;   \n"       \
    "                   ,           ,                :;%  %;   \n"        \
    "                    :         ;                   :;%;'     .,   \n" \
    "           ,.        %;     %;            ;        %;'    ,;   \n"   \
    "             ;       ;%;  %%;        ,     %;    ;%;    ,%'   \n"    \
    "              %;       %;%;      ,  ;       %;  ;%;   ,%;'   \n"     \
    "               ;%;      %;        ;%;        % ;%;  ,%;'   \n"       \
    "                `%;.     ;%;     %;'         `;%%;.%;'   \n"         \
    "                 `:;%.    ;%%. %@;        %; ;@%;%'   \n"            \
    "                    `:%;.  :;bd%;          %;@%;'   \n"              \
    "                      `@%:.  :;%.         ;@@%;'   \n"               \
    "                        `@%.  `;@%.      ;@@%;         \n"           \
    "                          `@%%. `@%%    ;@@%;        \n"             \
    "                            ;@%. :@%%  %@@%;       \n"               \
    "                              %@bd%%%bd%%:;     \n"                  \
    "                                #@%%%%%:;;   \n"                     \
    "                                %@@%%%::;   \n"                      \
    "                                %@@@%(o);  . '         \n"           \
    "                                %@@@o%;:(.,'         \n"             \
    "                            `.. %@@@o%::;         \n"                \
    "                               `)@@@o%::;         \n"                \
    "                                %@@(o)::;        \n"                 \
    "                               .%@@@@%::;         \n"                \
    "                               ;%@@@@%::;.          \n"              \
    "                              ;%@@@@%%:;;;. \n"                      \
    "                          ...;%@@@@@%%:;;;;,.."

#include <fstream>
#include <iostream>

#include "AForm.hpp"

class Bureaucrat;
class AForm;

class ShrubberyCreationForm : public AForm {
   public:
    ShrubberyCreationForm();
    ShrubberyCreationForm(const std::string target);
    ShrubberyCreationForm(const ShrubberyCreationForm &other);
    ~ShrubberyCreationForm();
    ShrubberyCreationForm &operator=(const ShrubberyCreationForm &other);

    void               doExecute(void) const;
    const std::string &getTarget(void) const;

   private:
    const std::string _target;
};

#endif