#ifndef ITER_HPP
#define ITER_HPP

template <typename T, typename Function>
void iter(T* array, size_t n, Function func) {
    for (size_t i = 0; i < n; i++) {
        func(array[i]);
    }
}

template <typename T>
void iter(T* array, size_t n, void (*func)(T&)) {
    if (!func)
        return;
    for (size_t i = 0; i < n; i++) {
        func(array[i]);
    }
}

template <typename T>
void iter(T* array, size_t n, void (*func)(const T&)) {
    if (!func)
        return;
    for (size_t i = 0; i < n; i++) {
        func(array[i]);
    }
}

#endif