#include "../include/rsa.h"
#include "../include/util.h"

#include <vector>
#include <string>
#include <gmpxx.h>

using StrVec = std::vector<std::string>;
using IntVec = std::vector<mpz_class>;

IntVec rEncrypt(const std::string& plaintext, mpz_class n, mpz_class e) {
    StrVec blocks = get_blocks(plaintext, 214);
    IntVec cblocks;

    for (auto& block : blocks) {
        mpz_class bi;
        mpz_import(bi.get_mpz_t(),block.size(),1,1,0,0,block.data());
        mpz_class c;
        //mpz_powm(c.get_mpz_t(),bi.get_mpz_t(),e.get_mpz_t(),n.get_mpz_t());
        c = largeModularExponentiation(bi,e,n);
        cblocks.push_back(c);
    }
    return cblocks;
}

std::string rDecrypt(const IntVec& ciphertext, mpz_class n, mpz_class d) {
    std::string result;
    for (auto& block : ciphertext) {
        mpz_class p;
        //mpz_powm(p.get_mpz_t(),block.get_mpz_t(),d.get_mpz_t(),n.get_mpz_t());
        p = largeModularExponentiation(block,d,n);
        std::size_t count = 0;
        mpz_export(nullptr,&count,1,1,0,0,p.get_mpz_t());
        std::vector<unsigned char> out(count ? count : 1);
        mpz_export(out.data(),&count,1,1,0,0,p.get_mpz_t());
        std::string s(reinterpret_cast<char*>(out.data()),count);
        result.append(s);
    }
    return result;
}