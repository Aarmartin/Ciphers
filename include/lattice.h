#ifndef LATTICE_H
#define LATTICE_H

#include <vector>

using Vec = std::vector<int>;
using Mat = std::vector<Vec>;

#include <random>

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

class LWE {
public:
    LWE(int m, int n, int q, double sigma);

    void keygen(PublicKey &pk, PrivateKey &sk);

    CipherText encrypt(const PublicKey &pk, int msg);

    int decrypt(const PrivateKey &sk, const CipherText &ct);

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