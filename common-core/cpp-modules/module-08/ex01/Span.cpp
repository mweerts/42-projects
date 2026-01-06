#include "Span.hpp"

Span::Span(unsigned int n) : _maxSize(n) {}

Span::Span(const Span& other) {
    *this = other;
}

Span& Span::operator=(const Span& other) {
    if (this != &other) {
        _maxSize = other._maxSize;
        _numbers = other._numbers;
    }
    return *this;
}

Span::~Span() {}

void Span::addNumber(int number) {
    if (_numbers.size() >= _maxSize) {
        throw std::overflow_error("Span: max size reached");
    }
    _numbers.push_back(number);
}

int Span::shortestSpan() const {
    if (_numbers.size() < 2) {
        throw std::logic_error("Span: not enough numbers to find a span");
    }

    std::vector<int> sortedNumbers = _numbers;
    std::sort(sortedNumbers.begin(), sortedNumbers.end());

    int minSpan = sortedNumbers[1] - sortedNumbers[0];
    for (size_t i = 1; i < sortedNumbers.size() - 1; ++i) {
        int span = sortedNumbers[i + 1] - sortedNumbers[i];
        if (span < minSpan) {
            minSpan = span;
        }
    }
    return minSpan;
}

int Span::longestSpan() const {
    if (_numbers.size() < 2) {
        throw std::logic_error("Span: not enough numbers to find a span");
    }

	int minNumber = *std::min_element(_numbers.begin(), _numbers.end());
	int maxNumber = *std::max_element(_numbers.begin(), _numbers.end());

    return maxNumber - minNumber;
}
