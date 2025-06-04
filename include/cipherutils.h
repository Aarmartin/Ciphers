#ifndef CIPHER_UTIL_H
#define CIPHER_UTIL_H

#include <vector>
#include <string>
#include <gmpxx.h>

using Vec = std::vector<int>;
using Mat = std::vector<Vec>;

namespace CipherUtils {

    namespace Arithmetic {
        int gcd(int a, int b);
        int mod(int a, int m);
        int modInverse(int a, int m);
        int soc(int a, int m, int b, int n);
        int totient(int n);
        mpz_class modInverse(mpz_class a, mpz_class m);
        std::tuple<mpz_class,mpz_class> modInverseRecursiveLoop(mpz_class a, mpz_class m);
    }

    namespace VectorOps {
        void modVec(Vec& v, int q);
        Vec matVecMul(const Mat &m, const Vec &v, int q);
        Vec matTVecMul(const Mat &m, const Vec &v, int q);
        Vec vecAdd(const Vec &u, const Vec &v, int q);
        int dot(const Vec &u, const Vec &v, int q);
    }

    namespace Exponentiation {
        int modularExponentiation(int a, int e, int m);
        int fastModularExponentiation(int a, int e, int m);
        mpz_class largeModularExponentiation(mpz_class a, mpz_class e, mpz_class m);
    }

    namespace numberTheory {
        mpz_class jacobi(mpz_class m, mpz_class n);
    }

    namespace Primes {
        mpz_class generateNumber(size_t size);
        mpz_class generateLessThan(mpz_class &number);
        bool isPrime(mpz_class number);
        mpz_class generatePrime(size_t size);
    }

    namespace Text {
        char shiftChar(char c, int shift, int m);
        std::string strtolower(std::string str);
        std::string strtoupper(std::string str);
        std::string sanatizeBits(const std::string& text);
    }
}

#endif