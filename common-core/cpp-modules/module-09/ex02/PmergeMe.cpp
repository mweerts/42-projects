#include "PmergeMe.hpp"

#include <algorithm>
#include <iterator>

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other) {
    *this = other;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
    }
    return *this;
}

PmergeMe::~PmergeMe() {}


std::vector<size_t> PmergeMe::generateJacobsthalSequence(size_t n) {
    std::vector<size_t> jacobsthal;
    if (n == 0)
        return jacobsthal;

    size_t j1 = 1;
    size_t j2 = 1;
    jacobsthal.push_back(j1);

    while (j2 < n) {
        size_t next = j2 + 2 * j1;
        if (next >= n) {
            jacobsthal.push_back(n);
            break;
        }
        jacobsthal.push_back(next);
        j1 = j2;
        j2 = next;
    }

    return jacobsthal;
}


// VECTOR

size_t PmergeMe::binarySearch(const std::vector<int>& vec, int value,
                              size_t start, size_t end) {
    size_t left = start;
    size_t right = end;

    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (vec[mid] < value) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

void PmergeMe::insertionSort(std::vector<int>& vec, size_t start, size_t end) {
    for (size_t i = start + 1; i < end; ++i) {
        int    key = vec[i];
        size_t j = i;
        while (j > start && vec[j - 1] > key) {
            vec[j] = vec[j - 1];
            --j;
        }
        vec[j] = key;
    }
}

void PmergeMe::fordJohnsonSort(std::vector<int>& vec) {
    size_t n = vec.size();

    // if (n <= 20) {
    //     insertionSort(vec, 0, n);
    //     return;
    // }

    std::vector<std::pair<int, int> > pairs;
    int                               straggler = -1;
    bool                              hasStraggler = false;

    for (size_t i = 0; i + 1 < n; i += 2) {
        if (vec[i] < vec[i + 1]) {
            pairs.push_back(std::make_pair(vec[i + 1], vec[i]));
        } else {
            pairs.push_back(std::make_pair(vec[i], vec[i + 1]));
        }
    }

    if (n % 2 == 1) {
        straggler = vec[n - 1];
        hasStraggler = true;
    }

    for (size_t i = 1; i < pairs.size(); ++i) {
        std::pair<int, int> key = pairs[i];
        size_t              j = i;
        while (j > 0 && pairs[j - 1].first > key.first) {
            pairs[j] = pairs[j - 1];
            --j;
        }
        pairs[j] = key;
    }

    std::vector<int> mainChain;
    std::vector<int> pending;

    for (size_t i = 0; i < pairs.size(); ++i) {
        mainChain.push_back(pairs[i].first);
        pending.push_back(pairs[i].second);
    }

    if (!pending.empty()) {
        mainChain.insert(mainChain.begin(), pending[0]);
        pending.erase(pending.begin());
    }

    std::vector<size_t> jacobsthal = generateJacobsthalSequence(pending.size());
    std::vector<bool>   inserted(pending.size(), false);

    for (size_t i = 0; i < jacobsthal.size(); ++i) {
        size_t idx = jacobsthal[i];
        if (idx > 0)
            idx--;

        size_t prevIdx = (i == 0) ? 0 : jacobsthal[i - 1];

        for (size_t j = idx; j >= prevIdx && j < pending.size(); --j) {
            if (!inserted[j]) {
                size_t pos =
                    binarySearch(mainChain, pending[j], 0, mainChain.size());
                mainChain.insert(mainChain.begin() + pos, pending[j]);
                inserted[j] = true;
            }
            if (j == 0)
                break;
        }
    }

    if (hasStraggler) {
        size_t pos = binarySearch(mainChain, straggler, 0, mainChain.size());
        mainChain.insert(mainChain.begin() + pos, straggler);
    }

    vec = mainChain;
}

void PmergeMe::sort(std::vector<int>& vec) {
    fordJohnsonSort(vec);
}


// DEQUE

size_t PmergeMe::binarySearch(const std::deque<int>& deq, int value,
                              size_t start, size_t end) {
    size_t left = start;
    size_t right = end;

    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (deq[mid] < value) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

void PmergeMe::insertionSort(std::deque<int>& deq, size_t start, size_t end) {
    for (size_t i = start + 1; i < end; ++i) {
        int    key = deq[i];
        size_t j = i;
        while (j > start && deq[j - 1] > key) {
            deq[j] = deq[j - 1];
            --j;
        }
        deq[j] = key;
    }
}

void PmergeMe::fordJohnsonSort(std::deque<int>& deq) {
    size_t n = deq.size();

    // if (n <= 20) {
    //     insertionSort(deq, 0, n);
    //     return;
    // }

    std::deque<std::pair<int, int> > pairs;
    int                              straggler = -1;
    bool                             hasStraggler = false;

    for (size_t i = 0; i + 1 < n; i += 2) {
        if (deq[i] < deq[i + 1]) {
            pairs.push_back(std::make_pair(deq[i + 1], deq[i]));
        } else {
            pairs.push_back(std::make_pair(deq[i], deq[i + 1]));
        }
    }

    if (n % 2 == 1) {
        straggler = deq[n - 1];
        hasStraggler = true;
    }

    for (size_t i = 1; i < pairs.size(); ++i) {
        std::pair<int, int> key = pairs[i];
        size_t              j = i;
        while (j > 0 && pairs[j - 1].first > key.first) {
            pairs[j] = pairs[j - 1];
            --j;
        }
        pairs[j] = key;
    }

    std::deque<int> mainChain;
    std::deque<int> pending;

    for (size_t i = 0; i < pairs.size(); ++i) {
        mainChain.push_back(pairs[i].first);
        pending.push_back(pairs[i].second);
    }

    if (!pending.empty()) {
        mainChain.insert(mainChain.begin(), pending[0]);
        pending.erase(pending.begin());
    }

    std::vector<size_t> jacobsthal = generateJacobsthalSequence(pending.size());
    std::vector<bool>   inserted(pending.size(), false);

    for (size_t i = 0; i < jacobsthal.size(); ++i) {
        size_t idx = jacobsthal[i];
        if (idx > 0)
            idx--;

        size_t prevIdx = (i == 0) ? 0 : jacobsthal[i - 1];

        for (size_t j = idx; j >= prevIdx && j < pending.size(); --j) {
            if (!inserted[j]) {
                size_t pos =
                    binarySearch(mainChain, pending[j], 0, mainChain.size());
                mainChain.insert(mainChain.begin() + pos, pending[j]);
                inserted[j] = true;
            }
            if (j == 0)
                break;
        }
    }

    if (hasStraggler) {
        size_t pos = binarySearch(mainChain, straggler, 0, mainChain.size());
        mainChain.insert(mainChain.begin() + pos, straggler);
    }

    deq = mainChain;
}

void PmergeMe::sort(std::deque<int>& deq) {
    fordJohnsonSort(deq);
}
