#include "../../include/lattice.h"
#include "../../include/cipherutils.h"

#include <vector>
#include <string>
#include <random>
#include <iostream>

using namespace CipherUtils;

using Vec = std::vector<int>;
using Mat = std::vector<Vec>;

std::ostream& operator<<(std::ostream& os, const LWECipherText& ct) {
    os << ct.ct.size() << '\n';
    for (const auto &cb : ct.ct) {
        os << cb.c1.size() << ' ';
        for (int v : cb.c1) os << v << ' ';
        os << cb.c2.size() << ' ';
        for (int v : cb.c2) os << v << ' ';
        os << '\n';
    }
    return os;
}

std::istream& operator>>(std::istream& is, LWECipherText& ct) {
    size_t numBits;
    is >> numBits;
    ct.ct.resize(numBits);
    for (auto &cb : ct.ct) {
        size_t size_c1, size_c2;

        is >> size_c1;
        cb.c1.resize(size_c1);
        for (auto& v : cb.c1) is >> v;

        is >> size_c2;
        cb.c2.resize(size_c2);
        for (auto& v : cb.c2) is >> v;
    }

    return is;
}

std::ostream& operator<<(std::ostream& os, const LWEPrivateKey& sk) {
    os << sk.s.size() << "\n";
    for (int v : sk.s) os << v << " ";
    os << "\n";
    return os;
}

std::istream& operator>>(std::istream& is, LWEPrivateKey& sk) {
    size_t size;
    is >> size;
    sk.s.resize(size);
    for (int& v : sk.s) is >> v;
    return is;
}

std::ostream& operator<<(std::ostream& os, const LWEPublicKey& pk) {
    size_t rows = pk.A.size();
    size_t cols = (rows > 0) ? pk.A[0].size() : 0;

    os << rows << " " << cols << "\n";
    for (const auto& row : pk.A) {
        for (int v : row) os << v << " ";
        os << "\n";
    }

    os << pk.b.size() << "\n";
    for (int v : pk.b) os << v << " ";
    os << "\n";

    return os;
}

std::istream& operator>>(std::istream& is, LWEPublicKey& pk) {
    size_t rows, cols;
    is >> rows >> cols;
    pk.A.resize(rows, std::vector<int>(cols));
    for (auto& row : pk.A) {
        for (int& v : row) is >> v;
    }

    size_t bsize;
    is >> bsize;
    pk.b.resize(bsize);
    for (int& v : pk.b) is >> v;

    return is;
}

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

void LWE::keygen(LWEPublicKey &pk, LWEPrivateKey &sk) {
    sk.s.resize(n);
    for (auto &si : sk.s) si = sampleTrinary();

    pk.A.assign(m, Vec(n));
    for (auto &row : pk.A)
        for (auto &aij : row)
            aij = Arithmetic::mod(sampleUniform(),q);

    Vec e(m);
    for (auto &ei : e) ei = sampleGaussian();

    Vec As = VectorOps::matVecMul(pk.A, sk.s, q);
    pk.b.resize(m);
    for (int j = 0; j < m; j++) {
        pk.b[j] = Arithmetic::mod(As[j]+e[j], q);
    }
}

LWECipherBit LWE::encryptBit(const LWEPublicKey &pk, int msg) {
    Vec r(m);
    for (auto &ri : r) ri = sampleBinary();

    Vec e1(n),e2(1);
    for (auto &x : e1) x = sampleGaussian();
    e2[0] = sampleGaussian();
    Vec c1 = VectorOps::matTVecMul(pk.A, r, q); // Transpose A Matrix Vector Multiplication
    for (int i = 0; i < n; i++) c1[i] = Arithmetic::mod(c1[i] + e1[i], q);

    int br = VectorOps::dot(pk.b,r,q);
    int c2 = Arithmetic::mod(br + e2[0] + (q/2)*msg, q);

    return {c1, Vec{c2}};
}

int LWE::decryptBit(const LWEPrivateKey &sk, const LWECipherBit &ct) {
    int sci = VectorOps::dot(sk.s,ct.c1, q);
    int u = Arithmetic::mod(ct.c2[0] - sci, q);

    return (u > q/4 && u < 3*q/4) ? 1 : 0;
}

LWECipherText LWE::encrypt(const LWEPublicKey &pk, const std::string& plaintext) {
    LWECipherText result;
    for (char bit : plaintext) {
        result.ct.push_back(encryptBit(pk, bit - '0'));
    }
    return result;
}

std::string LWE::decrypt(const LWEPrivateKey &sk, const LWECipherText& ciphertext) {
    std::string result;
    int i;
    for (LWECipherBit ct : ciphertext.ct) {
        i = decryptBit(sk, ct);
        result += static_cast<char>(i + '0');
    }
    return result;
}