#ifndef CIPHER_H
#define CIPHER_H

#include <string>
#include <fstream>

class Cipher {
public:
    virtual ~Cipher() = default;

    virtual std::string encrypt(const std::string& plaintext) const = 0;

    virtual std::string decrypt(const std::string& ciphertext) const = 0;

    virtual void encryptFile(const std::string& inputPath,
                             const std::string& outputPath) const {
        std::ifstream in(inputPath, std::ios::binary);
        if (!in) throw std::runtime_error("Cannot open file for encryption");
        std::string data{std::istreambuf_iterator<char>(in),
                         std::istreambuf_iterator<char>()};
        in.close();

        std::ofstream out(outputPath, std::ios::binary);
        if (!out) throw std::runtime_error("Cannout open output file");
        out << encrypt(data);
        out.close();
    }

    virtual void decryptFile(const std::string& inputPath,
                             const std::string& outputPath) const {
        std::ifstream in(inputPath, std::ios::binary);
        if (!in) throw std::runtime_error("Cannot open file for encryption");
        std::string data{std::istreambuf_iterator<char>(in),
                         std::istreambuf_iterator<char>()};
        in.close();

        std::ofstream out(outputPath, std::ios::binary);
        if (!out) throw std::runtime_error("Cannot open output file");
        out << decrypt(data);
        out.close();
    }
};

#endif