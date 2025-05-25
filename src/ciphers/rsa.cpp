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

IntVec RSA::encrypt(const std::string& plaintext, RSAPublicKey &pk) {
    StrVec blocks = get_blocks(plaintext, 214);
    IntVec cblocks;

    for (auto& block : blocks) {
        mpz_class bi;
        mpz_import(bi.get_mpz_t(),block.size(),1,1,0,0,block.data());
        mpz_class c;
        //mpz_powm(c.get_mpz_t(),bi.get_mpz_t(),e.get_mpz_t(),n.get_mpz_t());
        c = largeModularExponentiation(bi,pk.e,pk.n);
        cblocks.push_back(c);
    }
    return cblocks;
}

std::string RSA::decrypt(const IntVec& ciphertext, RSAPrivateKey &sk) {
    std::string result;
    for (auto& block : ciphertext) {
        mpz_class p;
        //mpz_powm(p.get_mpz_t(),block.get_mpz_t(),d.get_mpz_t(),n.get_mpz_t());
        p = largeModularExponentiation(block,sk.d,sk.n);
        std::size_t count = 0;
        mpz_export(nullptr,&count,1,1,0,0,p.get_mpz_t());
        std::vector<unsigned char> out(count ? count : 1);
        mpz_export(out.data(),&count,1,1,0,0,p.get_mpz_t());
        std::string s(reinterpret_cast<char*>(out.data()),count);
        result.append(s);
    }
    return result;
}