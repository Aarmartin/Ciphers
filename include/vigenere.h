#ifndef VIGENERE_H
#define VIGENERE_H

#include "cipher.h"
#include <string>

class VigenereCipher : public Cipher {
public:
    explicit VigenereCipher(std::string key);

    std::string encrypt(const std::string& plaintext) const override;
    std::string decrypt(const std::string& ciphertext) const override;

private:
    std::string key_;
};


/*

std::string vEncrypt(const std::string& plaintext, const std::string& key);

std::string vDecrypt(const std::string& ciphertext, const std::string& key);

*/
#endif