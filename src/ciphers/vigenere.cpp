#include "../../include/vigenere.h"
#include "../../include/cipherutils.h"

#include <string>
#include <cctype>

using namespace CharUtils;

VigenereCipher::VigenereCipher(std::string key)
    : key_(strtoupper(key))
{}

std::string VigenereCipher::encrypt(const std::string& plaintext) const {
    std::string result;
    int i = 0;
    char k;
    for (char c : plaintext) {
        if (isalpha(c)) {
            c = tolower(c);
            k = key_[i % key_.length()];
            result += shiftChar(c, k - 'A', 26);
            ++i;
        }
    }
    return result;
}

std::string VigenereCipher::decrypt(const std::string& ciphertext) const {
    std::string result;
    int i = 0;
    for (char c : ciphertext) {
        if (isalpha(c)) {
            c = toupper(c);
            char k = key_[i % key_.length()];
            result += shiftChar(c, k - 'A', 26);
            ++i;
        }
    }
    return result;
}