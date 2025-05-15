#ifndef AFFINE_H
#define AFFINE_H

#include "cipher.h"
#include <string>

struct AffineKey {
    int a;
    int b;
};

class AffineCipher : public Cipher {
public:
    explicit AffineCipher(AffineKey key);

    std::string encrypt(const std::string& plaintext) const override;
    std::string decrypt(const std::string& ciphertext) const override;

private:
    AffineKey key_;
    char encryptChar(char c) const;
    char decryptChar(char c) const;
};

/*
std::string aEncrypt(const std::string& plaintext, int a, int b);

std::string aDecrypt(const std::string& ciphertext, int a, int b);
*/
#endif