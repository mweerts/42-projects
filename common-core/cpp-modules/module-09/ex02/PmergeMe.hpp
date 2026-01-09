#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <cstddef>
#include <deque>
#include <vector>

class PmergeMe {
   public:
    static void sort(std::vector<int>& vec);
    static void sort(std::deque<int>& deq);

   private:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();

    static std::vector<size_t> generateJacobsthalSequence(size_t n);

    static void   fordJohnsonSort(std::vector<int>& vec);
    static void   insertionSort(std::vector<int>& vec, size_t start, size_t end);
    static size_t binarySearch(const std::vector<int>& vec, int value, size_t start,
                        size_t end);

    static void   fordJohnsonSort(std::deque<int>& deq);
    static void   insertionSort(std::deque<int>& deq, size_t start, size_t end);
    static size_t binarySearch(const std::deque<int>& deq, int value, size_t start,
                        size_t end);
};

#endif