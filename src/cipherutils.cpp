#include "../include/cipherutils.h"
#include <utility>
#include <vector>
#include <tuple>
#include <cmath>
#include <random>
#include <iostream>
#include <gmpxx.h>

namespace CipherUtils::Arithmetic {

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

    int soc(int a, int m, int b, int n) {
        int n_inv = modInverse(n, m);  // Find inverse of n (mod m)
        int m_inv = modInverse(m, n);  // Find inverse of m (mod n)
        return (a*n*n_inv + b*m*m_inv) % (m*n);
    }

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

    int totient(int n) {
        std::vector<int> factors = p_factors(n);
        double res = static_cast<double>(n);
        for (auto &p : factors) {
            res *= (1.0 - (1.0 / p));
        }
        return static_cast<int>(res + 0.5);
    }

    // Modular Inverse Initial Call
    mpz_class modInverse(mpz_class a, mpz_class m) {
        auto [x,y] = modInverseRecursiveLoop(a, m);
        x = (x + m) % m;    // Ensure positive result mod m for the first coefficient
        return x;
    }

    std::tuple<mpz_class,mpz_class> modInverseRecursiveLoop(mpz_class a, mpz_class m) {

        // Base Case
        if (m == 0) {
            return {1, 0};
        }

        mpz_class x, x_p, y, y_p, q, r;

        q = a / m;                                      // Find the quotient of the two values
        r = a % m;                                      // Find the remainder of the two values

        std::tie(x_p, y_p) = modInverseRecursiveLoop(m, r);  // Retrieve previous x and y values from one level down with recursive call on x and y

        // ax + my = g => ay' + m(y'- qx') = g
        x = y_p;                                        // x becomes y'
        y = x_p - y_p * q;                              // y becomes x' - y' * q

        return {x, y};                                  // Return new coefficients
    }

}

namespace CipherUtils::VectorOps {

    // Vector in Modular q
    void modVec(Vec& v, int q) {
        for (auto &x : v) x = Arithmetic::mod(x, q);
    }

    // Matrix Vector Multiplication in Modular q
    Vec matVecMul(const Mat &m, const Vec &v, int q) {
        Vec out(m.size(), 0);
        for (std::size_t i = 0; i < m.size(); i++) {
            for (std::size_t j = 0; j < v.size(); j++) {
                out[i] = Arithmetic::mod(out[i] + m[i][j] * v[j],q);
            }
        }
        return out;
    }

    // Matrix^T Vector Multiplication in Modular q
    Vec matTVecMul(const Mat &m, const Vec &v, int q) {
        Vec out(m.size(), 0);
        for (std::size_t i = 0; i < m.size(); i++) {
            for (std::size_t j = 0; j < v.size(); j++) {
                out[j] = Arithmetic::mod(out[j] + m[i][j] * v[i],q);
            }
        }
        return out;
    }

    // Vector Addition in Modular q
    Vec vecAdd(const Vec &u, const Vec &v, int q) {
        Vec out(v.size(),0);
        for (std::size_t i = 0; i < u.size(); i++) {
            out[i] = Arithmetic::mod(u[i] + v[i], q);
        }
        return out;
    }

    // Dot Product in Modular q
    int dot(const Vec &u, const Vec &v, int q) {
        int sum = 0;
        for (std::size_t i = 0; i < u.size(); i++) {
            sum = Arithmetic::mod(sum + u[i] * v[i], q);
        }
        return sum;
    }

}

namespace CipherUtils::Exponentiation {

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

    // Fast Modular Exponentiation
    int fastModularExponentiation(int a, int e, int m) {
        a %= m;                                 // Reduce base mod m
        if (Arithmetic::gcd(a,m) == 1) {                    // Euler theorem only works if base and mod are coprime
            int phi_m = totient(m);
            e %= phi_m;                         // Reduce exponent by totient
        }
        return modularExponentiation(a, e, m);  // Regular modular exponentiation
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

}

namespace CipherUtils::NumberTheory {

    mpz_class jacobi(mpz_class m, mpz_class n) {

        if (n == 0 || n % 2 == 0) throw std::invalid_argument("n must be odd and > 0");

        // Base Cases
        if (m == 0) return 0;
        if (m == 1) return 1;

        // Reduce Mod n
        m %= n;

        // Not relatively prime, return 0
        mpz_class gcd;
        mpz_gcd(gcd.get_mpz_t(),m.get_mpz_t(), n.get_mpz_t());
        if (gcd != 1) return 0;

        // Reduce powers of 2
        mpz_class result = 1;
        bool odd_twos = false;
        while (m % 2 == 0) {
            odd_twos ^= 1;
            m /= 2;
        }
        if (odd_twos && (n % 8 == 3 || n % 8 == 5)) result = -result;   // If there are an odd number of 2s, and 2/n & 8 === +- 3, add -1 to product
        if (m == 1) return result; // Return result if power of 2 reduction reduced to 1

        // Flip and reduce
        if (m % 4 == 3 && n % 4 == 3) result = -result;
        return result * jacobi(n % m, m);
    }

}

namespace CipherUtils::Primes {

    // Generate Large Number
    mpz_class generateNumber(size_t size) {
        std::random_device rd;
        std::mt19937_64 gen(rd());

        mpz_class result = 0;

        size_t blocks = size / 64;
        size_t r = size % 64;

        for (size_t i = 0; i < blocks; i++) {
            uint64_t bs = gen();
            result <<= 64;
            result += bs;
        }

        if (r > 0) {
            uint64_t block = gen();
            block &= ((uint64_t(1) << r) - 1);
            result <<= r;
            result += block;
        }

        result |= (mpz_class(1) << (size - 1));

        return result;
    }

    // Generate Large Number less than other Number
    mpz_class generateLessThan(mpz_class &number) {
        mpz_class candidate;
        do
        {
            candidate = generateNumber(mpz_sizeinbase(number.get_mpz_t(), 2));
        } while (candidate >= number);
        return candidate;
    }

    // Calculate k from Error Bound
    int find_k(size_t size) {
        int k = std::ceil(std::log2((static_cast<double>(size)*std::log(2) - 2)/(0.00000000000001)) - 1);
        return k; 
    }

    // Test for Primality
    bool isPrime(mpz_class number) {
        // Edge cases
        if (number < 2) return false;
        if (number == 2) return true;
        if (number % 2 == 0) return false;
        size_t size = mpz_sizeinbase(number.get_mpz_t(), 2);
        mpz_class witness;
        mpz_class gcd;
        mpz_class abs;
        mpz_class euler;
        mpz_class j;

        // Calculate value of k for guaranteed probability
        int k = find_k(size);
        for (int i = 0; i < k; i++) {
            // Generate witness
            do
            {
                witness = generateNumber(size);
            } while (witness >= number || witness == 0);

            // Check gcd
            mpz_gcd(gcd.get_mpz_t(), witness.get_mpz_t(), number.get_mpz_t());
            if (gcd != 1) return false;

            // Check Jacobi symbol
            euler = Exponentiation::largeModularExponentiation(witness, (number-1)/2,number);
            j = NumberTheory::jacobi(witness, number);
            if (j == -1) j = number - 1;
            if (euler != j) return false;
        }
        return true;
    }

    // Generate Large Prime
    mpz_class generatePrime(size_t size) {
        mpz_class candidate;
        int count = 0;
        do
        {
            count++;
            candidate = generateNumber(size);
            candidate |= (mpz_class(1) << (size - 2));  // Top two bits to 1 (Distance of p from power of 2)
            candidate |= 1; // Last bit is 1 for odd
        } while (!isPrime(candidate));
        std::cout << "Tested: " << count << " numbers." << std::endl;
        std::cout << "Utilized " << find_k(size) << " rounds of witnesses with a probability of error: " << (static_cast<double>(size)*std::log(2) - 2) / (static_cast<double>(size)*std::log(2) - 2 + (std::pow(2,55 + 1))) << std::endl;
        return candidate;
    }

    // Generate Sophie Germain Prime
    mpz_class generateSophieGermainPrime(size_t size) {
        mpz_class candidate;
        do
        {
            candidate = generatePrime(size);
        } while (!isPrime(candidate * 2 + 1));
        return candidate;
    }
}

namespace CipherUtils::Text {

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

    std::string sanatizeBits(const std::string& text) {
        std::string bitstring;
        for (char c : text) {
            if (c == '0' || c == '1') bitstring += c;
        }
        return bitstring;
    }
}