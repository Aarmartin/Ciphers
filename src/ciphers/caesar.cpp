using namespace std;

#include "../../include/caesar.h"
#include "../../include/util.h"

string cEncrypt(const string& plaintext, int key) {
    string result;
    for(char c: plaintext) {
        if(isalpha(c)){
            c = tolower(c);
            result += char(((c - 'a' + key) % 26) + 'A');
        }
    }
    return result;
}

string cDecrypt(const string& ciphertext, int key) {
    string result;
    result = cEncrypt(ciphertext,26 - key);
    return strToLower(result);
}