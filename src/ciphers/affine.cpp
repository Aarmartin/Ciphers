#include "../../include/affine.h"
#include "../../include/cipherutils.h"
#include <string>
#include <random>
#include <iostream>

using namespace CipherUtils;

std::ostream& operator<<(std::ostream& os, const AffineKey& k) {
    os << k.a << ' ' << k.b;
    return os;
}
std::istream& operator>>(std::istream& is, AffineKey& k) {
    is >> k.a;
    is >> k.b;
    return is;
}

AffineCipher::AffineCipher() {}

void AffineCipher::keygen(AffineKey &k) {
    std::vector<int> z26star;

    for (int i = 1; i < 26; i++) {
        if (Arithmetic::gcd(i, 26) == 1) z26star.push_back(i);
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, z26star.size() - 1);
    std::uniform_int_distribution<> full(0, 25);

    k.a = z26star[dist(gen)];
    k.b = full(gen);
}

std::string AffineCipher::encrypt(const std::string& plaintext, const AffineKey &k) const {
    std::string result;
    for (char c : plaintext) {
        if (isalpha(c)) {
            c = toupper(c);
            result += encryptChar(c, k);
        }
    }
    return result;
}

std::string AffineCipher::decrypt(const std::string& ciphertext, const AffineKey &k) const {
    std::string result;
    for (char c : ciphertext) {
        if (isalpha(c)) {
            c = tolower(c);
            result += decryptChar(c, k);
        }
    }
    return result;
}

char AffineCipher::encryptChar(char c, const AffineKey &k) const {
    char base = std::isupper(c) ? 'A' : 'a';
    return char(((((c - base) * k.a) + k.b) % 26) + base);
}

char AffineCipher::decryptChar(char c, const AffineKey &k) const {
    char base = std::isupper(c) ? 'A' : 'a';
    int a_inv = Arithmetic::modInverse(k.a, 26);
    return char(((((((c - base) - k.b) * a_inv) % 26) + 26) % 26) + base);
}