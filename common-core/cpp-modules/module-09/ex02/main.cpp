#include <sys/time.h>

#include <ctime>
#include <deque>
#include <iostream>
#include <sstream>
#include <vector>

#include "PmergeMe.hpp"

static void printVector(const std::vector<int>& vec, const size_t limit) {
    for (size_t i = 0; i < vec.size() && i < limit; ++i) {
        std::cout << vec[i] << " ";
    }
    if (vec.size() > limit) {
        std::cout << "[...]";
    }
    std::cout << std::endl;
}

static bool parseInput(std::vector<int>& vec, std::deque<int>& deq, int argc,
                       char** argv) {
    for (int i = 1; i < argc; ++i) {
        std::istringstream iss(argv[i]);
        int                num = 0;

        iss >> num;
        if (iss.fail() || !iss.eof() || num < 0) {
            std::cerr << "Error: Invalid input '" << argv[i]
                      << "'. Only positive integers are allowed." << std::endl;
            return false;
        }
        vec.push_back(num);
        deq.push_back(num);
    }
    return true;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <list of positive integers>"
                  << std::endl;
        return 1;
    }

    std::vector<int> vec;
    std::deque<int>  deq;

    if (!parseInput(vec, deq, argc, argv)) {
        return 1;
    }

    std::cout << "Before:\t";
    printVector(vec, 10);

    double vec_time = 0.0;
    double deq_time = 0.0;

    PmergeMe sorter;

    {
        struct timeval start, end;
        gettimeofday(&start, NULL);
        sorter.sort(vec);
        gettimeofday(&end, NULL);
        vec_time = (end.tv_sec - start.tv_sec) * 1000000.0 +
                   (end.tv_usec - start.tv_usec);
    }

    {
        struct timeval start, end;
        gettimeofday(&start, NULL);
        sorter.sort(deq);
        gettimeofday(&end, NULL);
        deq_time = (end.tv_sec - start.tv_sec) * 1000000.0 +
                   (end.tv_usec - start.tv_usec);
    }

    std::cout << "After:\t";
    printVector(vec, 10);

    std::cout << "Time to process a range of " << vec.size()
              << " elements with std::vector : " << vec_time << " μs ("
              << vec_time * 0.001 << " ms)" << std::endl;
    std::cout << "Time to process a range of " << deq.size()
              << " elements with std::deque  : " << deq_time << " μs" << " ("
              << deq_time * 0.001 << " ms)" << std::endl;
    return 0;
}