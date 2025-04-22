#include "Fixed.hpp"

class Point {
   private:
    const Fixed _x;
    const Fixed _y;

   public:
    Point();
    Point(const float x, const float y);
    Point(const Point &other);
    Point &operator=(Point &other);
    ~Point();

    Fixed getX();
    Fixed getY();
};
