#include "../../include/affine.h"
#include "../../include/cipherutils.h"
#include <string>

using namespace CipherUtils;

AffineCipher::AffineCipher(AffineKey key)
    : key_(key)
{}

std::string AffineCipher::encrypt(const std::string& plaintext) const {
    std::string result;
    for (char c : plaintext) {
        if (isalpha(c)) {
            c = toupper(c);
            result += encryptChar(c);
        }
    }
    return result;
}

std::string AffineCipher::decrypt(const std::string& ciphertext) const {
    std::string result;
    int a_inv = modInverse(key_.a,26);
    for (char c : ciphertext) {
        if (isalpha(c)) {
            c = tolower(c);
            result += decryptChar(c);
        }
    }
    return result;
}

char AffineCipher::encryptChar(char c) const {
    char base = std::isupper(c) ? 'A' : 'a';
    return ((((c - base) * key_.a) + key_.b) % 26) + base;
}

char AffineCipher::decryptChar(char c) const {
    char base = std::isupper(c) ? 'A' : 'a';
    int a_inv = modInverse(key_.a, 26);
    return ((((((c - base) * a_inv) - key_.b) % 26) + 26) % 26);
}