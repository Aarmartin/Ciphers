using namespace std;

#include <iostream>
#include <fstream>
#include <vector>
#include "../include/caesar.h"

int main(int argc, char** argv) {

    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <filename>"
             << endl;
        return 1;
    }

    ifstream f;

    f.open(argv[1]);

    string text;
    string line;

    if (!f.is_open()) {
        cout << "File was not found" << endl;
        return 1;
    }

    while (getline(f,line)) {
        if (!f.eof()) {
            line += ' ';
        }
        text += line;
    }

    for (int i = 1; i < 26; i++) {
        cout << "Decrypted with Key " << i << ": " << cDecrypt(text,i) << endl;
    }

    return 0;
}