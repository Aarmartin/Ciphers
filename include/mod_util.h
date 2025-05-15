#ifndef MOD_UTIL_H
#define MOD_UTIL_H

#include <vector>

using Vec = std::vector<int>;
using Mat = std::vector<Vec>;

int gcd(int a, int b);

int mod(int a, int m);

int modInverse(int a, int m);

void modVec(Vec& v, int q);

Vec matVecMul(const Mat &m, const Vec &v, int q);

Vec matTVecMul(const Mat &m, const Vec &v, int q);

Vec vecAdd(const Vec &u, const Vec &v, int q);

int dot(const Vec &u, const Vec &v, int q);

#endif