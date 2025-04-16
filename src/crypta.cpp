using namespace std;

#include <iostream>
#include <fstream>
#include "../include/util.h"
#include "../include/ciphers.h"

void analyze_cipher(const string& text) {
    
    vector<float> english_freq = getEnglishFrequency();

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

void analyze_vigenere(const string& text) {
    cout << "Not Implemented" << endl;
}

void analyze_affine(const string& text) {
    
    float miofcv;
    float mmiofc = 0;
    int ma, mb;
    string plaintext;
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 26; j++) {
            if (gcd(j,26) == 1) {
                plaintext = aDecrypt(text,j,i);
                miofcv = miofc(frequency(plaintext),getEnglishFrequency());
                if (miofcv > mmiofc) {
                    mmiofc = miofcv;
                    ma = j;
                    mb = i;
                }
            }
        }
    }

    cout << "Key: (" << ma << "," << mb << ")" << endl
         << "Decrypted Text: " << aDecrypt(text,ma,mb) << endl;

    /*
    vector<float> freq = frequency(text);

    auto [m1, m2] = mostCommon(freq);

    cout << "Most Common Found: " << m1 << ", " << char(m1 + 'a') << ", " << freq[m1] << endl;
    cout << "Second Most Common Found: " << m2 << ", " << char(m2 + 'a') << ", " << freq[m2] << endl;

    auto [a, b] = affine_key('e','t',char(m1 + 'a'), char(m2 + 'a'));

    cout << "Key found: (" << a << "," << b << ")" << endl;
    cout << "Decrypted Text: " << aDecrypt(text, a, b) << endl;
    */
}

int main(int argc, char** argv) {

    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <cipher> <filename>"
             << endl;
        return 1;
    }

    string alg = argv[1];

    if (alg != "caesar" && alg != "vigenere" && alg != "affine") {
        cerr << "Enter a valid encryption method. (caesar, vigenere, affine)"
             << endl;
        return 1;
    }

    ifstream f;
    f.open(argv[2]);
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
    
    if (alg == "caesar") {
        analyze_cipher(text);
    }
    else if (alg == "vigenere") {
        analyze_vigenere(text);
    }
    else if (alg == "affine") {
        analyze_affine(text);
    }

    return 0;
}