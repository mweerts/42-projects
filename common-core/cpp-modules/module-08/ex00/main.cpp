#include <deque>
#include <iostream>
#include <list>
#include <vector>

#include "easyfind.hpp"

int main(void) {
    {
        std::cout << "=== Testing with vector ===\n" << std::endl;
        std::vector<int> v;
        for (int i = 0; i < 5; ++i)
            v.push_back(i);
        const std::vector<int>& cv = v;

        std::cout << "Valid value\t: ";
        try {
            std::cout << *easyfind(v, 3) << std::endl;
        } catch (const NotFoundException& e) {
            std::cerr << e.what() << std::endl;
        }

        std::cout << "Invalid value\t: ";
        try {
            std::cout << *easyfind(v, 42) << std::endl;
        } catch (const NotFoundException& e) {
            std::cerr << e.what() << std::endl;
        }

        std::cout << "Valid value (const container)\t: ";
        try {
            std::cout << *easyfind(cv, 3) << std::endl;
        } catch (const NotFoundException& e) {
            std::cerr << e.what() << std::endl;
        }

        std::cout << "Invalid value (const container)\t: ";
        try {
            std::cout << *easyfind(cv, 42) << std::endl;
        } catch (const NotFoundException& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    {
        std::cout << "\n=== Testing with list ===\n" << std::endl;
        std::list<int> lst;

        for (int i = 0; i < 5; ++i)
            lst.push_back(i);

        const std::list<int>& clst = lst;

        std::cout << "Valid value\t: ";
        try {
            std::cout << *easyfind(lst, 3) << std::endl;
        } catch (const NotFoundException& e) {
            std::cerr << e.what() << std::endl;
        }

        std::cout << "Invalid value\t: ";
        try {
            std::cout << *easyfind(lst, 42) << std::endl;
        } catch (const NotFoundException& e) {
            std::cerr << e.what() << std::endl;
        }

        std::cout << "Valid value (const container)\t: ";
        try {
            std::cout << *easyfind(clst, 3) << std::endl;
        } catch (const NotFoundException& e) {
            std::cerr << e.what() << std::endl;
        }

        std::cout << "Invalid value (const container)\t: ";
        try {
            std::cout << *easyfind(clst, 42) << std::endl;
        } catch (const NotFoundException& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    {
        std::cout << "\n=== Testing with deque ===\n" << std::endl;
        std::deque<int> dq;

        for (int i = 0; i < 5; ++i)
            dq.push_back(i);
        const std::deque<int>& cdq = dq;

        std::cout << "Valid value\t: ";
        try {
            std::cout << *easyfind(dq, 3) << std::endl;
        } catch (const NotFoundException& e) {
            std::cerr << e.what() << std::endl;
        }

        std::cout << "Invalid value\t: ";
        try {
            std::cout << *easyfind(dq, 42) << std::endl;
        } catch (const NotFoundException& e) {
            std::cerr << e.what() << std::endl;
        }

        std::cout << "Valid value (const container)\t: ";
        try {
            std::cout << *easyfind(cdq, 3) << std::endl;
        } catch (const NotFoundException& e) {
            std::cerr << e.what() << std::endl;
        }

        std::cout << "Invalid value (const container)\t: ";

        try {
            std::cout << *easyfind(cdq, 42) << std::endl;
        } catch (const NotFoundException& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    return 0;
}
