#ifndef RSA_H
#define RSA_H

#include "cipherutils.h"

#include <string>
#include <vector>
#include <gmpxx.h>

using IntVec = std::vector<mpz_class>;

struct RSAPublicKey {
    mpz_class n;
    mpz_class e;
};

struct RSAPrivateKey {
    mpz_class n;
    mpz_class d;
};

struct RSACipherText {
    IntVec ct;
};

std::ostream& operator<<(std::ostream& os, const RSAPrivateKey& sk);
std::istream& operator>>(std::istream& is, RSAPrivateKey& sk);
std::ostream& operator<<(std::ostream& os, const RSAPublicKey& pk);
std::istream& operator>>(std::istream& is, RSAPublicKey& pk);
std::ostream& operator<<(std::ostream& os, const RSACipherText& ct);
std::istream& operator>>(std::istream& is, RSACipherText& ct);

class RSA {
public:
    explicit RSA();

    void keygen(RSAPublicKey& pk, RSAPrivateKey& sk);
    IntVec encrypt(const std::string& plaintext, RSAPublicKey &pk);
    std::string decrypt(const RSACipherText& ciphertext, RSAPrivateKey &sk);

private:
    std::vector<std::string> get_blocks(const std::string& plaintext, std::size_t size = 214);
};

#endif