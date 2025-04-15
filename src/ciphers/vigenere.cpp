using namespace std;

#include <string>
#include <cctype>
#include "../../include/vigenere.h"

string vEncrypt(const string& plaintext, const string& key){
    string result;
    int i = 0;
    for (char c: plaintext){
        if (isalpha(c)){
            c = tolower(c);
            char k = toupper(key[i % key.length()]);
            result += char((((c - 'a') + (k - 'A')) % 26) + 'A');
            i++;
        }
    }
    return result;
}

string vDecrypt(const string& ciphertext, const string& key){
    string result;
    int i = 0;
    for (char c: ciphertext){
        if (isalpha(c)) {
            c = toupper(c);
            char k = toupper(key[i % key.length()]);
            result += char(((((c - 'A') - (k - 'A')) + 26) % 26) + 'a');
            i++;
        }
    }
    return result;
}