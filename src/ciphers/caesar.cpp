#include "../../include/caesar.h"
#include "../../include/cipherutils.h"
#include <string>

using namespace CharUtils;

CaesarCipher::CaesarCipher(int key)
    : key_( ((key % 26) + 26) % 26)
{}

std::string CaesarCipher::encrypt(const std::string& plaintext) const {
    std::string result;
    result.reserve(plaintext.size());
    for (char c : plaintext) {
        if (isalpha(c)) {
            c = tolower(c);
            result += shiftChar(c, key_, 26);
        }
        else result += c;
    }
    return result;
}

std::string CaesarCipher::decrypt(const std::string& ciphertext) const {
    CaesarCipher reverse(26 - key_);
    return strtolower(reverse.encrypt(ciphertext));
}