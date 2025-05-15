#include "../include/char_util.h"
#include <string>
#include <cctype>

static char shiftChar(char c, int shift) {
    if (!isalpha(c)) return c;
    shift = ((shift % 26) + 26) % 26;
    const char base = std::isupper(c) ? 'A' : 'a';
    return char((c - base + shift) % 26 + base);
}

std::string strtolower(std::string str) {
    std::string result;
    result.reserve(str.size());
    for (char& c : str) {
        result += tolower(c);
    }
    return result;
}

std::string strtoupper(std::string str) {
    std::string result;
    result.reserve(str.size());
    for (char& c : str) {
        result += toupper(c);
    }
    return result;
}