#include "utils.h"

bool isLetter(char ch) {
    return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z';
}

bool isDigit(char ch) {
    return '0' <= ch && ch <= '9';
}

bool isValidNameStart(char ch) {
    return isLetter(ch) || ch == '_' || ch == '$';
}

bool isValidNameExtension(char ch) {
    return isValidNameStart(ch) || isDigit(ch);
}

long naiveIntegerPow(long a, long n) {
    long m = 1;
    for (long i = 0; i < n; i++)
        m *= a;
    return m;
}
