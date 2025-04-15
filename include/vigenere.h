#ifndef VIGENERE_H
#define VIGENERE_H

#include <string>

std::string vEncrypt(const std::string& plaintext, const std::string& key);

std::string vDecrypt(const std::string& ciphertext, const std::string& key);

#endif