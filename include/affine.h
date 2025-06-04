#ifndef AFFINE_H
#define AFFINE_H

#include <string>

struct AffineKey {
    int a;
    int b;
};

std::ostream& operator<<(std::ostream& os, const AffineKey& k);
std::istream& operator>>(std::istream& is, AffineKey& k);

class AffineCipher {
public:
    explicit AffineCipher();

    void keygen(AffineKey &k);
    std::string encrypt(const std::string& plaintext, const AffineKey &k) const;
    std::string decrypt(const std::string& ciphertext, const AffineKey &k) const;

private:
    char encryptChar(char c, const AffineKey &k) const;
    char decryptChar(char c, const AffineKey &k) const;
};

#endif