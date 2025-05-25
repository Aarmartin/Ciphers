#ifndef RSA_H
#define RSA_H

#include "cipherutils.h"

#include <string>
#include <gmpxx.h>

struct RSAPublicKey {
    mpz_class n;
    mpz_class e;
};

struct RSAPrivateKey {
    mpz_class n;
    mpz_class d;
};

std::ostream& operator<<(std::ostream& os, const RSAPrivateKey& sk);
std::istream& operator>>(std::istream& is, RSAPrivateKey& sk);
std::ostream& operator<<(std::ostream& os, const RSAPublicKey& pk);
std::istream& operator>>(std::istream& is, RSAPublicKey& pk);

class RSA {
public:
    explicit RSA();

    void keygen(mpz_class& key);
    std::vector<mpz_class> encrypt(const std::string& plaintext, RSAPublicKey &pk);
    std::string decrypt(const std::vector<mpz_class>& ciphertext, RSAPrivateKey &sk);

private:
    std::vector<std::string> get_blocks(const std::string& plaintext, std::size_t size = 214);
};

#endif