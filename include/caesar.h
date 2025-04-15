#ifndef CAESAR_H
#define CAESAR_H

#include <string>

std::string cEncrypt(const std::string& plaintext, int key);

std::string cDecrypt(const std::string& ciphertext, int key);

#endif