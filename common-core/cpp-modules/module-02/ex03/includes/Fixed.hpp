#ifndef FIXED_HPP
#define FIXED_HPP

#include <iostream>

class Fixed {
   public:
    Fixed();
    Fixed(const Fixed &other);
    Fixed(const int number);
    Fixed(const float number);
    ~Fixed();
    Fixed &operator=(const Fixed &other);

    int    getRawBits(void) const;
    void   setRawBits(int const raw);
    int    toInt(void) const;
    float  toFloat(void) const;
    static Fixed &min(Fixed &nb1, Fixed &nb2);
    static const Fixed &min(const Fixed &nb1, const Fixed &nb2);
    static Fixed &max(Fixed &nb1, Fixed &nb2);
    static const Fixed &max(const Fixed &nb1, const Fixed &nb2);

    bool operator>(const Fixed &obj) const;
    bool operator<(const Fixed &obj) const;
    bool operator>=(const Fixed &obj) const;
    bool operator<=(const Fixed &obj) const;
    bool operator==(const Fixed &obj) const;
    bool operator!=(const Fixed &obj) const;

    Fixed operator+(const Fixed &obj) const;
    Fixed operator-(const Fixed &obj) const;
    Fixed operator*(const Fixed &obj) const;
    Fixed operator/(const Fixed &obj) const;

    Fixed operator++(void);
    Fixed operator++(int);
    Fixed operator--(void);
    Fixed operator--(int);



   private:
    int              _rawBits;
    static const int _fractionalBits = 8;
};

std::ostream &operator<<(std::ostream &out,const Fixed &obj);

#endif