#ifndef RSA_H
#define RSA_H

#include <vector>
#include <random>
#include <iostream>

using Vec = std::vector<int>;
using Mat = std::vector<Vec>;

struct PublicKey {
    Mat A;
    Vec b;
};

struct PrivateKey {
    Vec s;
};

struct CipherText {
    Vec c1;
    Vec c2;
};

std::ostream& operator<<(std::ostream& os, const CipherText& ct);
std::istream& operator>>(std::istream& is, CipherText& ct);
std::ostream& operator<<(std::ostream& os, const PrivateKey& sk);
std::istream& operator>>(std::istream& is, PrivateKey& sk);
std::ostream& operator<<(std::ostream& os, const PublicKey& pk);
std::istream& operator>>(std::istream& is, PublicKey& pk);

class LWE {
public:
    explicit LWE(int m, int n, int q, double sigma);

    void keygen(PublicKey &pk, PrivateKey &sk);
    CipherText encryptBit(const PublicKey &pk, int msg);
    int decryptBit(const PrivateKey &sk, const CipherText &ct);
    std::vector<CipherText> encrypt(const PublicKey &pk, const std::string& plaintext);
    std::string decrypt(const PrivateKey &sk, const std::vector<CipherText>& ciphertext);

private:
    int m, n;
    int q;
    double sigma;
    std::mt19937 rng;
    int max_gauss_radius;

    std::uniform_int_distribution<int> uni_dist;
    std::discrete_distribution<double> gauss_dist;
    std::uniform_int_distribution<int> tri_dist;
    std::uniform_int_distribution<int> bin_dist;

    int sampleUniform() {return uni_dist(rng);}
    int sampleTrinary() {return tri_dist(rng);}
    int sampleBinary() {return bin_dist(rng);}
    int sampleGaussian() {
        int idx = gauss_dist(rng);
        return idx - max_gauss_radius;
    }
};

#endif