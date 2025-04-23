#include "../includes/Fixed.hpp"

#include <cmath>

#include "Fixed.hpp"

// CONSTRUCTORS

Fixed::Fixed() {
    _rawBits = 0;
}

Fixed::Fixed(const Fixed &other) {
    *this = other;
}

Fixed::Fixed(const int number) {
    _rawBits = number << _fractionalBits;
}

Fixed::Fixed(const float number) {
    _rawBits = roundf(number * (1 << _fractionalBits));
}

// COPY ASSIGNMENT OPERATOR

Fixed &Fixed::operator=(const Fixed &other) {
    if (this != &other) {
        _rawBits = other.getRawBits();
    }
    return *this;
}

// DESTRUCTOR

Fixed::~Fixed() {}

// METHODS

int Fixed::getRawBits(void) const {
    return _rawBits;
}

void Fixed::setRawBits(int const raw) {
    _rawBits = raw;
}

int Fixed::toInt(void) const {
    return (getRawBits() >> _fractionalBits);
}

float Fixed::toFloat(void) const {
    return ((float)_rawBits / (float)(1 << _fractionalBits));
}

Fixed &Fixed::min(Fixed &nb1, Fixed &nb2) {
    if (nb1.getRawBits() < nb2.getRawBits())
        return (nb1);
    return (nb2);
}

const Fixed &Fixed::min(const Fixed &nb1, const Fixed &nb2) {
    if (nb1.getRawBits() < nb2.getRawBits())
        return (nb1);
    return (nb2);
}

Fixed &Fixed::max(Fixed &nb1, Fixed &nb2) {
    if (nb1.getRawBits() > nb2.getRawBits())
        return (nb1);
    return (nb2);
}

const Fixed &Fixed::max(const Fixed &nb1, const Fixed &nb2) {
    if (nb1.getRawBits() > nb2.getRawBits())
        return (nb1);
    return (nb2);
}

// INSERTION OPERATOR OVERLOAD

std::ostream &operator<<(std::ostream &out, const Fixed &obj) {
    out << obj.toFloat();
    return (out);
}

// COMPARISON OPERATORS OVERLOAD

bool Fixed::operator>(const Fixed &obj) const {
    return (_rawBits > obj.getRawBits());
}

bool Fixed::operator<(const Fixed &obj) const {
    return (_rawBits < obj.getRawBits());
}

bool Fixed::operator>=(const Fixed &obj) const {
    return (_rawBits >= obj.getRawBits());
}

bool Fixed::operator<=(const Fixed &obj) const {
    return (_rawBits <= obj.getRawBits());
}

bool Fixed::operator==(const Fixed &obj) const {
    return (_rawBits == obj.getRawBits());
}

bool Fixed::operator!=(const Fixed &obj) const {
    return (_rawBits != obj.getRawBits());
}

// ARITHMETIC OPERATORS

Fixed Fixed::operator+(const Fixed &obj) const {
    return (Fixed(this->toFloat() + obj.toFloat()));
}

Fixed Fixed::operator-(const Fixed &obj) const {
    return (Fixed(this->toFloat() - obj.toFloat()));
}

Fixed Fixed::operator*(const Fixed &obj) const {
    return (Fixed(this->toFloat() * obj.toFloat()));
}

Fixed Fixed::operator/(const Fixed &obj) const {
    return (Fixed(this->toFloat() / obj.toFloat()));
}

// INCREMENT/DECREMENT OPERATORS

Fixed Fixed::operator++(void) {
    ++this->_rawBits;
    return (*this);
}

Fixed Fixed::operator++(int) {
    Fixed tmp = *this;
    ++this->_rawBits;
    return (tmp);
}

Fixed Fixed::operator--(void) {
    --this->_rawBits;
    return (*this);
}

Fixed Fixed::operator--(int) {
    Fixed tmp = *this;
    --this->_rawBits;
    return (tmp);
}