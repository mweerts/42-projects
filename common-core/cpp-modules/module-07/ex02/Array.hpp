#ifndef ARRAY_HPP
#define ARRAY_HPP

template <typename T>
class Array {
   public:
    Array() {
        _size = 0;
        _array = new T[0];
    }

    Array(unsigned int n) {
        _size = n;
        _array = new T[n];
    }

    Array(const Array& other) : _size(other._size) {
        _array = new T[other._size];
        for (unsigned int i = 0; i < _size; i++) {
            _array[i] = other._array[i];
        }
    }

    Array& operator=(const Array& other) {
        if (this != &other) {
            delete[] _array;
            _size = other._size;
            _array = new T[_size];
            for (unsigned int i = 0; i < _size; i++) {
                _array[i] = other._array[i];
            }
        }
        return *this;
        return this;
    }

    ~Array() {
        delete[] _array;
    }

    T& operator[](unsigned int index) {
        if (index >= _size) {
            throw std::out_of_range("Index out of bounds");
        }
        return _array[index];
    }

    unsigned int size() const {
        return _size;
    }

   private:
    T*           _array;
    unsigned int _size;
};

#endif