#ifndef SPAN_HPP
#define SPAN_HPP

#include <algorithm>
#include <stdexcept>
#include <vector>

class Span {
   public:
    Span(unsigned int n);
    Span(const Span& other);
    Span& operator=(const Span& other);
    ~Span();

    void addNumber(int number);

    template <typename Interator>
    void addRange(Interator begin, Interator end) {
        int rangeSize = std::distance(begin, end);
        if (_numbers.size() + rangeSize > _maxSize) {
            throw std::overflow_error("Span: max size reached");
        }
        _numbers.insert(_numbers.end(), begin, end);
    }

    int  shortestSpan() const;
    int  longestSpan() const;

   private:
    unsigned int     _maxSize;
    std::vector<int> _numbers;
};

#endif