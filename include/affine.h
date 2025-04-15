#ifndef AFFINE_H
#define AFFINE_H

#include <string>

std::string aEncrypt(const std::string& plaintext, int a, int b);

std::string aDecrypt(const std::string& ciphertext, int a, int b);

#endif