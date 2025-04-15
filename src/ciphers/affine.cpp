using namespace std;

#include "../../include/affine.h"
#include "../../include/util.h"

string aEncrypt(const string& plaintext, int a, int b) {
    string result;
    for (char c: plaintext) {
        if (isalpha(c)) {
            c = tolower(c);
            result += char(((((c - 'a') * a) + b) % 26) + 'A');
        }
    }
    return result;
}

string aDecrypt(const string& ciphertext, int a, int b) {
    string result;
    int na = modInverse(a,26);
    for (char c: ciphertext) {
        if (isalpha(c)) {
            c = toupper(c);
            result += char((((na * ((c - 'A') - b)) % 26 + 26) % 26) + 'a');
        }
    }
    return result;
}