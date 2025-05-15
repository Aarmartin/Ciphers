using namespace std;

#include "../../include/caesar.h"
#include "../../include/char_util.h"

CaesarCipher::CaesarCipher(int key)
    : key_( ((key % 26) + 26) % 26)
{}

string CaesarCipher::encrypt(const string& plaintext) const {
    string result;
    result.reserve(plaintext.size());
    for (char c : plaintext) {
        if (isalpha(c)) {
            c = tolower(c);
            result += shiftChar(c, key_);
        }
        else result += c;
    }
    return result;
}

string CaesarCipher::decrypt(const string& ciphertext) const {
    CaesarCipher reverse(26 - key_);
    return strtolower(reverse.encrypt(ciphertext));
}