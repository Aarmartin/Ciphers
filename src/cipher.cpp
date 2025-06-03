#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <gmpxx.h>
#include "../include/ciphers.h"

std::string load_text(const std::string &fname) {
    std::ifstream f(fname);
    if (!f) {
        std::cerr << "Failed to open text file." << std::endl;
        return "";
    }
    std::string line;
    std::string text;
    while (std::getline(f, line)) {
        text += line;
    }
    return text;
}

void main_lwe_encrypt(const std::string &fname, const std::string &kname) {
    LWEPublicKey pk;

    std::ifstream keyFile(kname);
    if (!keyFile) {
        std::cerr << "Could not open Key file." << std::endl;
        return;
    }
    keyFile >> pk;
    if (!keyFile) {
        std::cerr << "Failed to deserialize public key from key file." << std::endl;
        return;
    }

    LWE cipher(1024, 512, 4093, 3.19);
    std::string text = load_text(fname);
    std::vector<CipherText> ct = cipher.encrypt(pk, text);

    std::string oname = std::filesystem::path(fname).replace_extension(".enc").string();
    std::ofstream outputFile(oname);
    if (!outputFile) {
        std::cerr << "Could not open output file." << std::endl;
        return;
    }
    for (CipherText &c : ct) {
        outputFile << c << "\n";
    }
}

void main_lwe_decrypt(const std::string &fname, const std::string &kname) {
    LWEPrivateKey sk;

    std::ifstream keyFile(kname);
    if (!keyFile) {
        std::cerr << "Could not open Key file." << std::endl;
        return;
    }
    keyFile >> sk;
    if (!keyFile) {
        std::cerr << "Failed to deserialize private key from key file." << std::endl;
        return;
    }
    LWE cipher(1024, 512, 4093, 3.19);
    std::ifstream cipherFile(fname);
    if (!cipherFile) {
        std::cerr << "Could not open ciphertext file." << std::endl;
        return;
    }

    std::vector<CipherText> ct;
    CipherText c;
    while (cipherFile >> c) {
        ct.push_back(c);
    }

    if (ct.empty()) {
        std::cerr << "No CipherText read from file." << std::endl;
        return;
    }

    std::string plaintext = cipher.decrypt(sk, ct);

    std::string oname = std::filesystem::path(fname).replace_extension(".dec").string();
    std::ofstream outputFile(oname);
    if (!outputFile) {
        std::cerr << "Could not open output file." << std::endl;
        return;
    }

    outputFile << plaintext;
}

void main_rsa_encrypt(const std::string &fname, const std::string &kname) {
    RSAPublicKey pk;
    std::ifstream keyFile(kname);
    if (!keyFile) {
        std::cerr << "Could not open Key file." << std::endl;
        return;
    }
    keyFile >> pk;
    if (!keyFile) {
        std::cerr << "Failed to deserialize public key from key file." << std::endl;
        return;
    }

    std::string text = load_text(fname);

    RSA cipher;

    RSACipherText ct;
    ct.ct = cipher.encrypt(text, pk);

    std::string oname = std::filesystem::path(fname).replace_extension(".enc").string();
    std::ofstream outputFile(oname);
    if (!outputFile) {
        std::cerr << "Could not open output file." << std::endl;
        return;
    }

    outputFile << ct;
}

void main_rsa_decrypt(const std::string &fname, const std::string &kname) {
    RSAPrivateKey sk;
    std::ifstream keyFile(kname);
    if (!keyFile) {
        std::cerr << "Could not open Key file." << std::endl;
        return;
    }
    keyFile >> sk;
    if (!keyFile) {
        std::cerr << "Failed to deserialize public key from key file." << std::endl;
        return;
    }

    std::ifstream cipherFile(fname);
    if (!cipherFile) {
        std::cerr << "Could not open ciphertext file." << std::endl;
        return;
    }

    RSACipherText ct;

    cipherFile >> ct;

    RSA cipher;

    std::string plaintext = cipher.decrypt(ct, sk);

    std::string oname = std::filesystem::path(fname).replace_extension(".dec").string();
    std::ofstream outputFile(oname);
    if (!outputFile) {
        std::cerr << "Could not open output file." << std::endl;
        return;
    }
    outputFile << plaintext;
}

int main(int argc, char** argv){

    if (argc != 5) {
        std::cerr << "Usage:" << argv[0]
             << " <[lwe|rsa]> <[encrypt|decrypt]> <filename> <keyfile>"
             << std::endl;
        return -1;
    }

    std::string alg = argv[1];
    std::string op = argv[2];
    std::string fname = argv[3];
    std::string kname = argv[4];

    if (alg != "lwe" && alg != "rsa") {
        std::cerr << "Enter a valid encryption method. (lwe, rsa)"
             << std::endl;
        return 1;
    }

    if (op != "encrypt" && op != "decrypt") {
        std::cerr << "Enter a valid operation. (encrypt, decrypt)"
             << std::endl;
        return 1;
    }

    if (alg == "lwe") {
        if (op == "encrypt") {
            main_lwe_encrypt(fname, kname);
        } else {
            main_lwe_decrypt(fname, kname);
        }
    } else if (alg == "rsa") {
        if (op == "encrypt") {
            main_rsa_encrypt(fname, kname);
        } else {
            main_rsa_decrypt(fname, kname);
        }
    }

    return 0;
}

