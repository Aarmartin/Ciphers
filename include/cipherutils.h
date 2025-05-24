#ifndef CIPHER_UTIL_H
#define CIPHER_UTIL_H

#include <vector>
#include <string>

using Vec = std::vector<int>;
using Mat = std::vector<Vec>;

namespace CipherUtils {

    int gcd(int a, int b);

    int mod(int a, int m);

    int modInverse(int a, int m);

    void modVec(Vec& v, int q);

    Vec matVecMul(const Mat &m, const Vec &v, int q);

    Vec matTVecMul(const Mat &m, const Vec &v, int q);

    Vec vecAdd(const Vec &u, const Vec &v, int q);

    int dot(const Vec &u, const Vec &v, int q);

}

namespace CharUtils {

    static char shiftChar(char c, int shift, int m);

    std::string strtolower(std::string str);

    std::string strtoupper(std::string str);
}

#endif