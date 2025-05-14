#ifndef LATTICE_H
#define LATTICE_H

#include <vector>

using Int = int;
using Vec = vector<int>;
using Mat = vector<Vec>;

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
    LWE(int n, int m, Int q, double sigma);

    void keygen(PublicKey &pk, PrivateKey &sk);

    CipherText encrypt(const PublicKey &pk, int msg);

    int decrypt(const PrivateKey &sk, const CipherText &ct);

private:
    int n, m;
    Int q;
    double sigma;
    std::mt19937 rng;

    std::uniform_int_distribution<Int> uni_dist;
    std::normal_distribution<double> gauss_dist;
    std::discrete_distribution<Int> tri_dist;

    Int sampleUniform() {return uni_dist(rng);}
    Int sampleGaussian() {return static_cast<Int>(std::round(gauss_dist(rng)));}
    Int sampleTrinary() {
        Int x = tri_dist(rng);
        return (x == 0 ? -1 : x == 1 ? 0 : 1);
    }

    Int mod(Int a, Int m);
    void modVec(Vec &v, Int q);
    Vec matVecMul(const Mat &m,const Vec &v, Int q);
    Vec vecAdd(const Vec &a, const Vec &b);
    Int dot(const Vec &a, const Vec &b, Int q); 
};

#endif