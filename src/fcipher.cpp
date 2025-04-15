using namespace std;

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include "../include/caesar.h"
#include "../include/vigenere.h"
#include "../include/affine.h"
#include "../include/util.h"

void main_caesar(const string& op, const string& text, const string& fname) {

    int key;
    bool enc = op == "encrypt";

    cout << "Enter your key: ";
    cin >> key;
    cin.ignore();

    if (enc) {
        string ciphertext = cEncrypt(text,key);
        string oname = filesystem::path(fname).replace_extension(".enc").string();
        ofstream of(oname);
        if (!of) {
            cerr << "Could not open output file" << endl;
        }
        of << ciphertext;
        of.close();
    }
    else {
        string plaintext = cDecrypt(text,key);
        string oname = filesystem::path(fname).replace_extension(".dec").string();
        ofstream of(oname);
        if (!of) {
            cerr << "Could not open output file" << endl;
        }
        of << plaintext;
        of.close();
    }

}

void main_vigenere(const string& op, const string& text, const string& fname) {

    string key;
    bool enc = op == "encrypt";

    cout << "Enter your key: ";
    getline(cin,key);

    if (enc) {
        string ciphertext = vEncrypt(text,key);
        string oname = filesystem::path(fname).replace_extension(".enc").string();
        ofstream of(oname);
        if (!of) {
            cerr << "Could not open output file" << endl;
        }
        of << ciphertext;
        of.close();
    }
    else {
        string plaintext = vDecrypt(text,key);
        string oname = filesystem::path(fname).replace_extension(".dec").string();
        ofstream of(oname);
        if (!of) {
            cerr << "Could not open output file" << endl;
        }
        of << plaintext;
        of.close();
    }
}

void main_affine(const string& op, const string& text, const string& fname) {
    string key;
    int a;
    int b;
    bool enc = op == "encrypt";

    cout << "Enter your key a,b: ";
    cin >> key;
    cin.ignore();

    a = stoi(key.substr(0,key.find(',')));
    b = stoi(key.substr(key.find(',') + 1));

    if (gcd(a,26) != 1) {
        cout << "Enter a valid key (First value must be coprime with 26)"
             << endl;
        return;
    }

    if (enc) {
        string ciphertext = aEncrypt(text,a,b);
        string oname = filesystem::path(fname).replace_extension(".enc").string();
        ofstream of(oname);
        if (!of) {
            cerr << "Could not open output file" << endl;
        }
        of << ciphertext;
        of.close();
    }
    else {
        string plaintext = aDecrypt(text,a,b);
        string oname = filesystem::path(fname).replace_extension(".dec").string();
        ofstream of(oname);
        if (!of) {
            cerr << "Could not open output file" << endl;
        }
        of << plaintext;
        of.close();
    }
}

int main(int argc, char** argv){

    if (argc != 4) {
        cerr << "Usage:" << argv[0]
             << " <encryption algorithm> <[encrypt|decrypt]> <filename>"
             << endl;
        return 1;
    }

    string alg = argv[1];
    string op = argv[2];
    string fname = argv[3];
    string line;
    string text;
    ifstream f;
    f.open(fname);
    if (!f.is_open()) {
        cout << "File not found"
             << endl;
        return 1;
    }
    while (getline(f,line)) {
        if (!f.eof()) {
            line += ' ';
        }
        text += line;
    }

    if (alg != "caesar" && alg != "vigenere" && alg != "affine") {
        cerr << "Enter a valid encryption method. (caesar, vigenere, affine)"
             << endl;
        return 1;
    }

    if (op != "encrypt" && op != "decrypt") {
        cerr << "Enter a valid operation. (encrypt, decrypt)"
             << endl;
        return 1;
    }

    if (alg == "caesar") {
        main_caesar(op, text, fname);
    }
    else if (alg == "vigenere") {
        main_vigenere(op, text, fname);
    }
    else if (alg == "affine") {
        main_affine(op, text, fname);
    }

    return 0;
}

