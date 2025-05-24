#ifndef CAESAR_H
#define CAESAR_H

#include <string>

class CaesarCipher {
public:
    explicit CaesarCipher(int key);

    std::string encrypt(const std::string& plaintext) const;
    std::string decrypt(const std::string& ciphertext) const;

private:
    int key_;
};

#endif