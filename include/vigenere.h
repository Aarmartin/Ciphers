#ifndef VIGENERE_H
#define VIGENERE_H

#include <string>

class VigenereCipher {
public:
    explicit VigenereCipher(std::string key);

    std::string encrypt(const std::string& plaintext) const;
    std::string decrypt(const std::string& ciphertext) const;

private:
    std::string key_;
};

#endif