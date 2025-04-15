using namespace std;

#include <iostream>
#include <fstream>
#include "../include/util.h"
#include "../include/caesar.h"

void analyze(const string& text) {
    
    vector<float> english_freq = {			
        0.082, /* a */
        0.015, /* b */
        0.028, /* c */
        0.043, /* d */ 
        0.127, /* e */
        0.022, /* f */
        0.020, /* g */
        0.061, /* h */
        0.070, /* i */
        0.002, /* j */
        0.008, /* k */
        0.040, /* l */
        0.024, /* m */
        0.067, /* n */
        0.075, /* o */
        0.019, /* p */
        0.001, /* q */
        0.060, /* r */
        0.063, /* s */
        0.091, /* t */
        0.028, /* u */
        0.010, /* v */
        0.023, /* w */
        0.001, /* x */
        0.020, /* y */
        0.001  /* z */
    };

    int imax = 0;
    float max = 0;
    vector<float> freq = frequency(text);
    for (int i = 0; i < 26; i++) {
        float mutual = miofc(freq,shift(english_freq,i));
        cout << "Mutual found for " << i << ": " << mutual << endl;
        if (mutual > max) {
            max = mutual;
            imax = i;
        }
    }
    cout << "Index Found: " << imax << endl;
    cout << "Decrypted Text: " << cDecrypt(text,imax) << endl;
}

int main(int argc, char** argv) {

    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <filename>"
             << endl;
        return 1;
    }

    ifstream f;
    f.open(argv[1]);
    if (!f.is_open()) {
        cout << "File not found"
             << endl;
        return 1;
    }

    string line;
    string text;

    while (getline(f,line)) {
        if (!f.eof()) {
            line += ' ';
        }
        text += line;
    }

    analyze(text);

    return 0;
}