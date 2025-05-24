#ifndef RSA_H
#define RSA_H

#include <string>
#include <gmpxx.h>

std::vector<mpz_class> rEncrypt(const std::string& plaintext, mpz_class n, mpz_class e);

std::string rDecrypt(const std::vector<mpz_class>& ciphertext, mpz_class n, mpz_class d);

#endif