using namespace std;

#include <vector>
#include <iostream>

using Int = int;
using Vec = vector<int>;
using Mat = vector<Vec>;

#include <random>
#include "../../include/lattice.h"

LWE::LWE(int n_, int m_, Int q_, double sigma_)
    : n(n_), m(m_), q(q_), sigma(sigma_), rng(random_device{}()),
    uni_dist(0, q_-1),
    gauss_dist(0.0, sigma_),
    tri_dist({1.0/3, 1.0/3, 1.0/3}) {}

Int LWE::mod(Int a, Int m){
    Int r = a % m;
    return r < 0 ? r + m : r;
}

void LWE::modVec(Vec &v, Int q) {
    for (auto &x : v) x = mod(x, q);
}

Vec LWE::matVecMul(const Mat &m,const Vec &v, Int q) {
    Vec out(m.size(), 0);
    for (size_t i = 0; i < m.size(); i++) {
        for (size_t j = 0; j < v.size(); j++) {
            out[i] = mod(out[i] + m[i][j] * v[j],q);
        }
    }
    return out;
}

Int LWE::dot(const Vec &a, const Vec &b, Int q) {
    Int sum = 0;
    for (size_t i = 0; i < a.size(); i++) {
        sum = mod(sum + a[i] * b[i], q);
    }
    return sum;
}

void LWE::keygen(PublicKey &pk, PrivateKey &sk) {
    sk.s.resize(n);
    for (auto &si : sk.s) si = sampleTrinary();

    pk.A.assign(n, Vec(m));
    for (auto &row : pk.A)
        for (auto &aij : row)
            aij = sampleUniform();

    Vec e(m);
    for (auto &ei : e) ei = sampleGaussian();

    Vec As = matVecMul(pk.A, sk.s, q);
    pk.b.resize(m);
    for (int j = 0; j < m; j++) {
        pk.b[j] = mod(As[j]+e[j], q);
    }
}

CipherText LWE::encrypt(const PublicKey &pk, int msg) {
    Vec r(m);
    for (auto &ri : r) ri = sampleUniform();

    Vec e1(n),e2(1);
    for (auto &x : e1) x = sampleGaussian();
    e2[0] = sampleGaussian();
    for (auto &ei : e1) cout << ei << endl;
    Vec c1 = matVecMul(pk.A, r, q);
    for (int i = 0; i < n; i++) c1[i] = mod(c1[i] + e1[i], q);

    Int br = dot(pk.b,r,q);
    Int c2 = mod(br + e2[0] + (q/2)*msg, q);

    return {c1, Vec{c2}};
}

Int LWE::decrypt(const PrivateKey &sk, const CipherText &ct) {
    Int sci = dot(sk.s,ct.c1, q);
    Int u = mod(ct.c2[0] - sci, q);

    return (u > q/4 && u < 3*q/4) ? 1 : 0;
}