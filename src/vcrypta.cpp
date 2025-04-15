using namespace std;

#include <iostream>
#include <fstream>
#include "../include/util.h"
#include "../include/vigenere.h"

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

    int size = 0;
    float ioc;
    vector<float> freq;
    for (auto i = 1u; i < text.length(); i++) {
        freq = frequency(substring(text,i,0));
        ioc = iofc(freq);
        if (ioc > 0.064 && ioc < 0.066) {
            cout << "Key size: " << i << endl;
            size = i;
            break;
        }
    }
    int imax = 0;
    float max = 0;
    float mutual;
    string key;
    for (auto i = 0; i < size; i++) {
        freq = frequency(substring(text,size,i));
        for (int j = 0; j < 26; j++) {
            mutual = miofc(freq,shift(english_freq,j));
            cout << "Size: " << i << " Shift: " << j <<  " Mutual IOC: " << mutual << endl;
            if (mutual > max) {
                max = mutual;
                imax = j;
            }
        }
        cout << "Max found: " << imax << " Shift value: " << char(imax + 'A') << endl;
        key += char(imax + 'A');
    }

    cout << "Key: " << key << endl;    
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