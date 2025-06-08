#pragma once

#include <memory>
#include <cstring>

bool isLetter(char ch);

bool isDigit(char ch);

bool isValidNameStart(char ch);

bool isValidNameExtension(char ch);

long naiveIntegerPow(long a, long n);


template <class T2, class T1>
T2 bit_cast(T1 t1) {
    static_assert(sizeof(T1)==sizeof(T2), "Types must match sizes");
    static_assert(std::is_pod<T1>::value, "Requires POD input");
    static_assert(std::is_pod<T2>::value, "Requires POD output");

    T2 t2;
    std::memcpy( std::addressof(t2), std::addressof(t1), sizeof(T1) );
    return t2;
}