#include <vector>

using Vec = std::vector<int>;
using Mat = std::vector<Vec>;

#include <random>
#include "../../include/lattice.h"
#include "../../include/mod_util.h"

LWE::LWE(int m_, int n_, int q_, double sigma_)
    : m(m_), n(n_), q(q_), sigma(sigma_), rng(std::random_device{}()),
    uni_dist(0, q_-1),
    gauss_dist(),
    tri_dist(-1,1),
    bin_dist(0,1)
{
    max_gauss_radius = static_cast<int>(std::ceil(6.0 * sigma));

    std::vector<double> weights;
    weights.reserve(2*max_gauss_radius + 1);
    for (int k = -max_gauss_radius; k <= max_gauss_radius; ++k) {
        weights.push_back(std::exp(- (double)(k*k) / (2.0 * sigma * sigma)));
    }

    using Param = decltype(gauss_dist)::param_type;
    gauss_dist.param(Param(weights.begin(), weights.end()));
}

void LWE::keygen(PublicKey &pk, PrivateKey &sk) {
    sk.s.resize(n);
    for (auto &si : sk.s) si = sampleTrinary();

    pk.A.assign(m, Vec(n));
    for (auto &row : pk.A)
        for (auto &aij : row)
            aij = mod(sampleUniform(),q);

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
    for (auto &ri : r) ri = sampleBinary();

    Vec e1(n),e2(1);
    for (auto &x : e1) x = sampleGaussian();
    e2[0] = sampleGaussian();
    Vec c1 = matTVecMul(pk.A, r, q); // Transpose A Matrix Vector Multiplication
    for (int i = 0; i < n; i++) c1[i] = mod(c1[i] + e1[i], q);

    int br = dot(pk.b,r,q);
    int c2 = mod(br + e2[0] + (q/2)*msg, q);

    return {c1, Vec{c2}};
}

int LWE::decrypt(const PrivateKey &sk, const CipherText &ct) {
    int sci = dot(sk.s,ct.c1, q);
    int u = mod(ct.c2[0] - sci, q);

    return (u > q/4 && u < 3*q/4) ? 1 : 0;
}