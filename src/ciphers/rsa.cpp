#include "../../include/rsa.h"
//#include "../include/util.h"

#include <vector>
#include <string>
#include <iostream>
#include <gmpxx.h>

using namespace CipherUtils;

using StrVec = std::vector<std::string>;
using IntVec = std::vector<mpz_class>;

std::ostream& operator<<(std::ostream& os, const RSAPublicKey& pk) {
    os << pk.n.get_str(10) << "\n" << pk.e.get_str(10) << "\n";
    return os;
}

std::istream& operator>>(std::istream& is, RSAPublicKey& pk) {
    std::string n_str, e_str;
    if (!(std::getline(is, n_str) && std::getline(is, e_str))) {
        is.setstate(std::ios::failbit);
        return is;
    }
    pk.n.set_str(n_str, 10);
    pk.e.set_str(e_str, 10);
    return is;
}

std::ostream& operator<<(std::ostream& os, const RSAPrivateKey& sk) {
    os << sk.n.get_str(10) << "\n" << sk.d.get_str(10) << "\n";
    return os;
}

std::istream& operator>>(std::istream& is, RSAPrivateKey& sk) {
    std::string n_str, d_str;
    if (!(std::getline(is, n_str) && std::getline(is, d_str))) {
        is.setstate(std::ios::failbit);
        return is;
    }
    sk.n.set_str(n_str, 10);
    sk.d.set_str(d_str, 10);
    return is;
}

std::ostream& operator<<(std::ostream& os, const RSACipherText& ct) {
    os << ct.ct.size() << '\n';
    for (const auto& block : ct.ct) {
        os << block.get_str() << '\n';
    }
    return os;
}

std::istream& operator>>(std::istream& is, RSACipherText& ct) {
    size_t count;
    is >> count;
    ct.ct.clear();
    ct.ct.reserve(count);

    std::string block;
    for (size_t i = 0; i < count; i++) {
        is >> block;
        ct.ct.emplace_back(block);
    }
    return is;
}

StrVec RSA::get_blocks(const std::string& plaintext, std::size_t size) {
    StrVec blocks;
    for (std::size_t i = 0; i < plaintext.size(); i += size) {
        blocks.push_back(plaintext.substr(i, size));
    }
    std::string& le = blocks.back();
    if (le.size() < size) {
        le.resize(size, '\0');
    }
    return blocks;
}

RSA::RSA() {}

void RSA::keygen(RSAPublicKey &pk, RSAPrivateKey &sk) {
    // Generate 2 1024 bit primes
    std::cout << "**** Generating prime number p ****\n";
    mpz_class p = Primes::generatePrime(1024);
    std::cout << "\n**** Generating prime number q ****\n";
    mpz_class q = Primes::generatePrime(1024);

    // Calculate n and φ(n)
    mpz_class n = p * q;
    mpz_class phi_n = (p-1) * (q-1);

    // Find random value of e coprime with n
    mpz_class e;
    mpz_class gcd;
    do
    {
        e = Primes::generateLessThan(phi_n);
        mpz_gcd(gcd.get_mpz_t(), phi_n.get_mpz_t(), e.get_mpz_t());
    } while (gcd != 1);

    // Calculate e inverse d (mod φ(n))
    mpz_class d = Arithmetic::modInverse(e, phi_n);

    pk.n = n;
    pk.e = e;

    sk.n = n;
    sk.d = d;
}

IntVec RSA::encrypt(const std::string& plaintext, RSAPublicKey &pk) {
    StrVec blocks = get_blocks(plaintext, 214); // Separate into blocks
    IntVec cblocks;

    for (auto& block : blocks) {
        mpz_class bi;
        mpz_import(bi.get_mpz_t(),block.size(),1,1,0,0,block.data());
        mpz_class c;
        //mpz_powm(c.get_mpz_t(),bi.get_mpz_t(),e.get_mpz_t(),n.get_mpz_t());
        c = Exponentiation::largeModularExponentiation(bi,pk.e,pk.n);   // Calculate m^e (mod n)
        cblocks.push_back(c); // Push block onto ciphertext
    }
    return cblocks;
}

std::string RSA::decrypt(const RSACipherText& ciphertext, RSAPrivateKey &sk) {
    std::string result;
    for (auto& block : ciphertext.ct) {
        mpz_class p;
        //mpz_powm(p.get_mpz_t(),block.get_mpz_t(),d.get_mpz_t(),n.get_mpz_t());
        p = Exponentiation::largeModularExponentiation(block,sk.d,sk.n);    // Calculate c^d (mod n)
        std::size_t count = 0;
        mpz_export(nullptr,&count,1,1,0,0,p.get_mpz_t());
        std::vector<unsigned char> out(count ? count : 1);
        mpz_export(out.data(),&count,1,1,0,0,p.get_mpz_t());
        std::string s(reinterpret_cast<char*>(out.data()),count);
        result.append(s);
    }
    // Remove trailing null characters
    while (!result.empty() && result.back() == '\0') {
        result.pop_back();
    }
    return result;
}