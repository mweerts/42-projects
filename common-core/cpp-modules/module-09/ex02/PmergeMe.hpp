#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <cstddef>
#include <deque>
#include <vector>

class PmergeMe {
   public:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();

    void sort(std::vector<int>& vec);
    void sort(std::deque<int>& deq);

   private:
    std::vector<size_t> generateJacobsthalSequence(size_t n);

    void   fordJohnsonSort(std::vector<int>& vec);
    void   insertionSort(std::vector<int>& vec, size_t start, size_t end);
    size_t binarySearch(const std::vector<int>& vec, int value, size_t start,
                        size_t end);

    void   fordJohnsonSort(std::deque<int>& deq);
    void   insertionSort(std::deque<int>& deq, size_t start, size_t end);
    size_t binarySearch(const std::deque<int>& deq, int value, size_t start,
                        size_t end);
};

#endif