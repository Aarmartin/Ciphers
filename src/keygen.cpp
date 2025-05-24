#include "include/ciphers.h"

#include <iostream>
#include <filesystem>
#include <fstream>

void main_lwe_keygen(std::string fname) {

    std::string opname = std::filesystem::path(fname).replace_extension(".pk").string();
    std::string osname = std::filesystem::path(fname).replace_extension(".sk").string();

    std::ofstream pkf(opname);
    if (!pkf) {
        std::cerr << "Could not open Public Key File" << std::endl;
        return;
    }
    std::ofstream skf(osname);
    if (!skf) {
        std::cerr << "Could not open Private Key File" << std::endl;
        return;
    }

    LWE cipher(1024, 512, 4093, 3.19);

    PublicKey pk;
    PrivateKey sk;

    cipher.keygen(pk, sk);

    pkf << pk;
    skf << sk;

    pkf.close();
    skf.close();
}

int main(int argc, char** argv){

    if (argc != 3) {
        std::cerr << "Bad Parameters" << std::endl;
        return -1;
    }

    std::string cipher = argv[1];
    std::string fname = argv[2];

    if (cipher == "lwe") {
        main_lwe_keygen(fname);
    }

    return 1;
}