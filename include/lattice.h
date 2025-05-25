#ifndef LATTICE_H
#define LATTICE_H

#include <vector>
#include <random>
#include <iostream>

using Vec = std::vector<int>;
using Mat = std::vector<Vec>;

struct LWEPublicKey {
    Mat A;
    Vec b;
};

struct LWEPrivateKey {
    Vec s;
};

struct CipherText {
    Vec c1;
    Vec c2;
};

std::ostream& operator<<(std::ostream& os, const CipherText& ct);
std::istream& operator>>(std::istream& is, CipherText& ct);
std::ostream& operator<<(std::ostream& os, const LWEPrivateKey& sk);
std::istream& operator>>(std::istream& is, LWEPrivateKey& sk);
std::ostream& operator<<(std::ostream& os, const LWEPublicKey& pk);
std::istream& operator>>(std::istream& is, LWEPublicKey& pk);

class LWE {
public:
    explicit LWE(int m, int n, int q, double sigma);

    void keygen(LWEPublicKey &pk, LWEPrivateKey &sk);
    CipherText encryptBit(const LWEPublicKey &pk, int msg);
    int decryptBit(const LWEPrivateKey &sk, const CipherText &ct);
    std::vector<CipherText> encrypt(const LWEPublicKey &pk, const std::string& plaintext);
    std::string decrypt(const LWEPrivateKey &sk, const std::vector<CipherText>& ciphertext);

private:
    int m, n;
    int q;
    double sigma;
    std::mt19937 rng;
    int max_gauss_radius;

    std::uniform_int_distribution<int> uni_dist;
    std::discrete_distribution<int> gauss_dist;
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