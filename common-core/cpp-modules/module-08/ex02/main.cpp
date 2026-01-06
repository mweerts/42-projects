#include <iostream>
#include <list>
#include <vector>

#include "MutantStack.hpp"

int main() {
    {
        std::cout << "=== MutantStack test ===\n" << std::endl;

        MutantStack<int> mstack;
        mstack.push(5);
        mstack.push(17);
        std::cout << "TOP : " << mstack.top() << std::endl;
        mstack.pop();
        std::cout << "SIZE : " << mstack.size() << std::endl;
        mstack.push(3);
        mstack.push(5);
        mstack.push(737);
        mstack.push(0);
        MutantStack<int>::iterator it = mstack.begin();
        MutantStack<int>::iterator ite = mstack.end();
        ++it;
        --it;
        while (it != ite) {
            std::cout << *it << std::endl;
            ++it;
        }
    }

    {
        std::cout << "=== List test ===\n" << std::endl;

        std::list<int> lst;
        lst.push_back(5);
        lst.push_back(17);
        std::cout << "TOP : " << lst.back() << std::endl;
        lst.pop_back();
        std::cout << "SIZE : " << lst.size() << std::endl;
        lst.push_back(3);
        lst.push_back(5);
        lst.push_back(737);
        lst.push_back(0);
        std::list<int>::iterator it = lst.begin();
        std::list<int>::iterator ite = lst.end();
        ++it;
        --it;
        while (it != ite) {
            std::cout << *it << std::endl;
            ++it;
        }
    }

    {
        std::cout << "==== Vector test ====\n" << std::endl;
        std::vector<int> vec;
        vec.push_back(5);
        vec.push_back(17);
        std::cout << "TOP : " << vec.back() << std::endl;
        vec.pop_back();
        std::cout << "SIZE : " << vec.size() << std::endl;
        vec.push_back(3);
        vec.push_back(5);
        vec.push_back(737);
        vec.push_back(0);
        std::vector<int>::iterator it = vec.begin();
        std::vector<int>::iterator ite = vec.end();
        ++it;
        --it;
        while (it != ite) {
            std::cout << *it << std::endl;
            ++it;
        }
    }
    return 0;
}