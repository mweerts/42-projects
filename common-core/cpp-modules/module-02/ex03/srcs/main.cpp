#include <iostream>

#include "Point.hpp"

bool bsp(Point const a, Point const b, Point const c, Point const point);

int main(void) {
    // Définition des sommets du triangle
    Point a(0.0f, 0.0f);
    Point b(5.0f, 0.0f);
    Point c(2.5f, 5.0f);

    // Test 1 : Point à l'intérieur du triangle
    Point inside(2.5f, 2.0f);
    std::cout << "Test 1 (Inside): "
              << (bsp(a, b, c, inside) ? "True" : "False") << std::endl;

    // Test 2 : Point sur un bord du triangle
    Point onEdge(2.5f, 0.0f);
    std::cout << "Test 2 (On Edge): "
              << (bsp(a, b, c, onEdge) ? "True" : "False") << std::endl;

    // Test 3 : Point sur un sommet du triangle
    Point onVertex(0.0f, 0.0f);
    std::cout << "Test 3 (On Vertex): "
              << (bsp(a, b, c, onVertex) ? "True" : "False") << std::endl;

    // Test 4 : Point à l'extérieur du triangle
    Point outside(5.0f, 5.0f);
    std::cout << "Test 4 (Outside): "
              << (bsp(a, b, c, outside) ? "True" : "False") << std::endl;

    // Test 5 : Point très proche d'un bord mais à l'intérieur
    Point nearEdgeInside(2.5f, 0.003f);
    std::cout << "Test 5 (Near Edge Inside): "
              << (bsp(a, b, c, nearEdgeInside) ? "True" : "False") << std::endl;

    // Test 6 : Point très proche d'un bord mais à l'extérieur
    Point nearEdgeOutside(2.5f, -0.003f);
    std::cout << "Test 6 (Near Edge Outside): "
              << (bsp(a, b, c, nearEdgeOutside) ? "True" : "False") << std::endl;

    // Test 7 : Point colinéaire avec deux sommets mais à l'extérieur
    Point colinearOutside(7.5f, 0.0f);
    std::cout << "Test 7 (Colinear Outside): "
              << (bsp(a, b, c, colinearOutside) ? "True" : "False") << std::endl;

    // Test 8 : Triangle dégénéré (aire nulle) avec un point sur la ligne
    Point degenerateA(0.0f, 0.0f);
    Point degenerateB(5.0f, 0.0f);
    Point degenerateC(10.0f, 0.0f);
    Point onLine(2.5f, 0.0f);
    std::cout << "Test 8 (Degenerate Triangle, On Line): "
              << (bsp(degenerateA, degenerateB, degenerateC, onLine) ? "True" : "False") << std::endl;

    // Test 9 : Triangle dégénéré (aire nulle) avec un point hors de la ligne
    Point offLine(2.5f, 1.0f);
    std::cout << "Test 9 (Degenerate Triangle, Off Line): "
              << (bsp(degenerateA, degenerateB, degenerateC, offLine) ? "True" : "False") << std::endl;

    return 0;
}