#include <iostream>

#include "Fixed.hpp"

int main(void) {
    Fixed       a;
    Fixed const b(Fixed(5.05f) * Fixed(2));
    std::cout << a << std::endl;
    std::cout << ++a << std::endl;
    std::cout << a << std::endl;
    std::cout << a++ << std::endl;
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << Fixed::max(a, b) << std::endl;

    std::cout << std::endl;

    Fixed c(2.5f);
    Fixed d(1.5f);
    std::cout << "c  : " << c <<" d : " <<  d << std::endl;
    std::cout << "c + d : " << c + d << std::endl;
    std::cout << "c - d : " << c - d << std::endl;
    std::cout << "c * d : " << c * d << std::endl;
    std::cout << "c / d : " << c / d << std::endl;

    std::cout << std::endl;

    bool res = c < d;
    std::cout << "c < d : " << res << std::endl;
    res = c <= d;
    std::cout << "c <= d : " << res << std::endl;
    res = c > d;
    std::cout << "c > d : " << res << std::endl;
    res = c >= d;
    std::cout << "c >= d : " << res << std::endl;
    res = c == d;
    std::cout << "c == d : " << res << std::endl;
    res = c != d;
    std::cout << "c != d : " << res << std::endl;

    std::cout << std::endl;

    std::cout << "min(c, d) : " << Fixed::min(c, d) << std::endl;
    std::cout << "max(c, d) : " << Fixed::max(c, d) << std::endl;

    std::cout << std::endl;

    std::cout << "c : " << c << " ++c : " << ++c << std::endl;
    std::cout << "c : " << c << " c++ : " << c++ << std::endl;
    std::cout << "c : " << c << " --c : " << --c << std::endl;
    std::cout << "c : " << c << " c-- : " << c-- << std::endl;

    return 0;
}