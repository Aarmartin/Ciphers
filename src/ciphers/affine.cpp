using namespace std;

#include "../../include/affine.h"
#include "../../include/mod_util.h"

AffineCipher::AffineCipher(AffineKey key)
    : key_(key)
{}

std::string AffineCipher::encrypt(const std::string& plaintext) const {
    string result;
    for (char c : plaintext) {
        if (isalpha(c)) {
            c = toupper(c);
            result += encryptChar(c);
        }
    }
    return result;
}

std::string AffineCipher::decrypt(const std::string& ciphertext) const {
    string result;
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

/*
string aEncrypt(const string& plaintext, int a, int b) {
    string result;
    for (char c: plaintext) {
        if (isalpha(c)) {
            c = tolower(c);
            result += char(((((c - 'a') * a) + b) % 26) + 'A');
        }
    }
    return result;
}

string aDecrypt(const string& ciphertext, int a, int b) {
    string result;
    int na = modInverse(a,26);
    for (char c: ciphertext) {
        if (isalpha(c)) {
            c = toupper(c);
            result += char((((na * ((c - 'A') - b)) % 26 + 26) % 26) + 'a');
        }
    }
    return result;
}
    */