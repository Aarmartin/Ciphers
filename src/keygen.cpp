#include "../include/ciphers.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <set>

void print_usage() {
    std::cout << "Usage:\n"
              << "\tkeygen <cipher_type> <key_file>\n"
              << "Where:\n"
              << "\t<cipher_type>\t= rsa | lwe | affine\n"
              << "\t<key_file>\t= key file name\n";
}

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

    LWEPublicKey pk;
    LWEPrivateKey sk;

    cipher.keygen(pk, sk);

    pkf << pk;
    skf << sk;

    pkf.close();
    skf.close();
}

void main_rsa_keygen(std::string fname) {
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

    RSA cipher;

    RSAPublicKey pk;
    RSAPrivateKey sk;

    cipher.keygen(pk, sk);

    pkf << pk;
    skf << sk;

    pkf.close();
    skf.close();
}

void main_affine_keygen(std::string fname) {
    std::string oname = std::filesystem::path(fname).replace_extension(".key").string();

    std::ofstream kf(oname);
    if (!kf) {
        std::cerr << "Could not open Public Key File" << std::endl;
        return;
    }

    AffineCipher cipher;

    AffineKey k;

    cipher.keygen(k);

    kf << k;

    kf.close();
}

int main(int argc, char** argv){

    const std::set<std::string> valid_ciphers = {"rsa", "lwe", "affine"};

    if (argc != 3) {
        std::cerr << "Error: Incorrect number of arguments.\n";
        print_usage();
        return -1;
    }

    std::string cipher_type = argv[1];
    std::string fname = argv[2];

    if (valid_ciphers.find(cipher_type) == valid_ciphers.end()) {
        std::cerr << "Error: Invalid cipher type '" << cipher_type << "'.\n";
        print_usage();
        return -1;
    }

    if (cipher_type == "lwe") {
        main_lwe_keygen(fname);
    } else if (cipher_type == "rsa") {
        main_rsa_keygen(fname);
    } else if (cipher_type == "affine") {
        main_affine_keygen(fname);
    }

    return 1;
}