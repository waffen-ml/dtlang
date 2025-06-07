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

int naiveIntegerPow(int a, int n) {
    int m = 1;
    for (int i = 0; i < n; i++)
        m *= a;
    return m;
}

