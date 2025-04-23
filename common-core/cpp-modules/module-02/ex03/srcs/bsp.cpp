#include "Point.hpp"

static float triangleArea(Point a, Point b, Point c) {
    float area =
        (a.getX().toFloat() * (b.getY().toFloat() - c.getY().toFloat()) +
         b.getX().toFloat() * (c.getY().toFloat() - a.getY().toFloat()) +
         c.getX().toFloat() * (a.getY().toFloat() - b.getY().toFloat())) /
        2.0f;
	if (area < 0)
		return (-area);
    return area;
}

bool bsp(Point const a, Point const b, Point const c, Point const point) {
    float abcArea = triangleArea(a, b, c);
    float abpArea = triangleArea(a, b, point);
	float acpArea = triangleArea(a, c, point);
	float bcpArea = triangleArea(b, c, point);

	if (abpArea == 0.0f || acpArea == 0.0f || bcpArea == 0.0f)
		return (false);
	if (abpArea + acpArea + bcpArea == abcArea)
		return (true);
	return (false);
}