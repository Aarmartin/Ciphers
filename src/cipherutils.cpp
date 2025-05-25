#include "../include/cipherutils.h"
#include <utility>
#include <vector>
#include <cmath>
#include <gmpxx.h>

namespace CipherUtils {

    // Greatest Common Divisor
    int gcd(int a, int b) {
        int t;
        while (b != 0) {
            t = b;
            b = a % b;
            a = t;
        }
        return a;
    }

    // Modular value
    int mod(int a, int m) {
        int r = a % m;
        return r < 0 ? (r + m) % m : r;
    }

    // Modular Inverse
    int modInverse(int a, int m) {
        int m_orig = m;                         // Save original Mod value to account for negative result correction
        int x = 0;                              // Initial Coefficient of m
        int y = 1;                              // Initial Coefficient of a - Coefficients in the formula for GCD: ay' + mx' = g where x = y' = 1 and y = (y' - qx') = 0
        int q;

        while (a > 1) {                         // Continue until GCD of some value and 1
            q = a / m;                          // Find the quotient of the two values
            a = std::exchange(m, a % m);        // a becomes m, m becomes remainder f(a,m) -> f(m,r)

            // ax + my = g => ay' + m(y'- qx') = g
            // These coefficient terms are the previous and previous previous values
            y = std::exchange(x, y - q * x);    // x becomes y' - q * x', y becomes x'
        }

        y = (y + m_orig) % m_orig;              // Ensure positive result mod m for the second coefficient

        return y;                               // Return Coefficient
    }

    // Vector in Modular q
    void modVec(Vec& v, int q) {
        for (auto &x : v) x = mod(x, q);
    }

    // Matrix Vector Multiplication in Modular q
    Vec matVecMul(const Mat &m, const Vec &v, int q) {
        Vec out(m.size(), 0);
        for (std::size_t i = 0; i < m.size(); i++) {
            for (std::size_t j = 0; j < v.size(); j++) {
                out[i] = mod(out[i] + m[i][j] * v[j],q);
            }
        }
        return out;
    }

    // Matrix^T Vector Multiplication in Modular q
    Vec matTVecMul(const Mat &m, const Vec &v, int q) {
        Vec out(m.size(), 0);
        for (std::size_t i = 0; i < m.size(); i++) {
            for (std::size_t j = 0; j < v.size(); j++) {
                out[j] = mod(out[j] + m[i][j] * v[i],q);
            }
        }
        return out;
    }

    // Vector Addition in Modular q
    Vec vecAdd(const Vec &u, const Vec &v, int q) {
        Vec out(v.size(),0);
        for (std::size_t i = 0; i < u.size(); i++) {
            out[i] = mod(u[i] + v[i], q);
        }
        return out;
    }

    // Dot Product in Modular q
    int dot(const Vec &u, const Vec &v, int q) {
        int sum = 0;
        for (std::size_t i = 0; i < u.size(); i++) {
            sum = mod(sum + u[i] * v[i], q);
        }
        return sum;
    }

    // Prime Factors
    std::vector<int> p_factors(int n) {
        int i = 2;
        std::vector<int> factors;

        while (i * i <= n) {
            if (n % i != 0) {
                ++i;
            }
            else {
                n /= i;
                factors.push_back(i);
            }
        }
        if (n > 1) {
            factors.push_back(n);
        }
        sort(factors.begin(),factors.end());
        factors.erase(unique(factors.begin(),factors.end()),factors.end());
        return factors;
    }

    // Totient Function
    int totient(int n) {
        std::vector<int> factors = p_factors(n);
        double res = static_cast<double>(n);
        for (auto &p : factors) {
            res *= (1.0 - (1.0 / p));
        }
        return static_cast<int>(res + 0.5);
    }

    // Modular Exponentiation
    int modularExponentiation(int a, int e, int m) {
        int res = 1;
        a %= m;                                     // Reduce base mod m
        while (e > 0) {                             // While exponent is not 0
            if (e & 1) {                            // Check if least significant bit is a 1
                res = res * a % m;                  // Multiply current value of result by our current base mod m
            }
            a = static_cast<int>(pow(a,2)) % m;     // Square base to evaluate at next power of 2
            e >>= 1;                                // Shift exponent by 1 bit
        }
        return res;                                 // Resturn result
    }

    // Large Number Modular Exponentiation
    mpz_class largeModularExponentiation(mpz_class a, mpz_class e, mpz_class m) {
        mpz_class res("1");
        a %= m;
        while (e > 0) {
            if (e % 2 == 1) {
                res = res * a % m;
            }
            a = (a * a) % m;
            e >>= 1;
        }
        return res;
    }

    // Fast Modular Exponentiation
    int fastModularExponentiation(int a, int e, int m) {
        a %= m;                                 // Reduce base mod m
        if (gcd(a,m) == 1) {                    // Euler theorem only works if base and mod are coprime
            int phi_m = totient(m);
            e %= phi_m;                         // Reduce exponent by totient
        }
        return modularExponentiation(a, e, m);  // Regular modular exponentiation
    }
}

namespace CharUtils {

    char shiftChar(char c, int shift, int m) {
        if (!isalpha(c)) return c;
        shift = ((shift % m) + m) % m;
        const char base = std::isupper(c) ? 'A' : 'a';
        return char((c - base + shift) % m + base);
    }

    std::string strtolower(std::string str) {
        std::string result;
        result.reserve(str.size());
        for (char& c : str) {
            result += tolower(c);
        }
        return result;
    }

    std::string strtoupper(std::string str) {
        std::string result;
        result.reserve(str.size());
        for (char& c : str) {
            result += toupper(c);
        }
        return result;
    }
}