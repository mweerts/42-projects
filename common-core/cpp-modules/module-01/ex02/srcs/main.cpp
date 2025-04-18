#include <string>
#include <iostream>

int main(void) {
    std::string  stringVAR = "HI THIS IS BRAIN";
    std::string *stringPTR = &stringVAR;
    std::string &stringREF = stringVAR;

    std::cout << "Address of stringVAR : " << &stringVAR << std::endl;
	std::cout << "Address held by stringPTR : " << stringPTR << std::endl;
	std::cout << "Address held by stringREF : " << &stringREF << std::endl;

    std::cout << "Value of stringVAR : " << stringVAR << std::endl;
    std::cout << "Value pointed by stringPTR : " << *stringPTR << std::endl;
    std::cout << "Value pointed by stringREF : " << stringREF << std::endl;

}