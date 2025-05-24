#ifndef AFFINE_H
#define AFFINE_H

#include <string>

struct AffineKey {
    int a;
    int b;
};

class AffineCipher {
public:
    explicit AffineCipher(AffineKey key);

    std::string encrypt(const std::string& plaintext) const;
    std::string decrypt(const std::string& ciphertext) const;

private:
    AffineKey key_;
    char encryptChar(char c) const;
    char decryptChar(char c) const;
};

#endif