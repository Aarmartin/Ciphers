#ifndef CAESAR_H
#define CAESAR_H

#include "cipher.h"
#include <string>

class CaesarCipher : public Cipher {
public:
    explicit CaesarCipher(int key);

    std::string encrypt(const std::string& plaintext) const override;
    std::string decrypt(const std::string& ciphertext) const override;

private:
    int key_;
};

#endif