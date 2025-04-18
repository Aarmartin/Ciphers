using namespace std;

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include "../include/util.h"
#include "../include/ciphers.h"

void analyze_caesar(const string& text) {
    
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

void analyze_caesar_bruteforce(const string& text) {
    for (int i = 1; i < 26; i++) {
        cout << "Decrypted with Key " << i << ": " << cDecrypt(text,i) << endl;
    }
}
/*
void analyze_vigenere(const string& text) {

    vector<float> english_frequency = getEnglishFrequency();
    vector<float> freq;

    string stext;

    float ioc;
    float max_ioc;
    float max_max_ioc = 0;
    float avg_ioc;
    int kmax_max_ioc = 0;

    // Key Length
    for (int k = 2; k < 40; k++) {
        avg_ioc = 0;
        max_ioc = 0;
        for (int s = 0; s < k; s++) {
            stext = substring(text,k,s);
            freq = frequency(stext);
            ioc = iofc(freq);
            cout << "IOFC: " << ioc <<  endl;
            avg_ioc += ioc;
            if (ioc > max_ioc) {
                max_ioc = ioc;
            }
        }
        avg_ioc = avg_ioc / k;
        if (max_ioc > max_max_ioc) {
            max_max_ioc = max_ioc;
            kmax_max_ioc = k;
        }
        cout << "Key Length: " << k << " \tMax IOFC: " << max_ioc << " \tAverage IOFC: " << avg_ioc << endl;
    }

    cout << "Suspected Key Length: " << kmax_max_ioc << endl;


}*/

void analyze_vigenere(const string& text) {
    int n = text.length();
    float ic = iofc(frequency(text));

    int fk = int(((0.0265 * n) / ((ic * (n-1)) - 0.0385 * n + 0.065)) + 0.5f);

    cout << "Suspected Friedman Key Length: " << fk << " Adjusted: " << fk + int(sqrt(fk) + 4) << endl;

    vector<float> freq;
    float ioc;
    float best_ioc = 0;
    int best_k = 0;
    float avg_ioc;
    string stext;
    // Key Length
    for (int k = 2; k < fk + int(sqrt(fk) + 4); k++) {
        avg_ioc = 0;
        for (int s = 0; s < k; s++) {
            stext = substring(text,k,s);
            freq = frequency(stext);
            ioc = iofc(freq);
            avg_ioc += ioc;
        }
        avg_ioc = avg_ioc / k;
        if (avg_ioc > best_ioc) {
            best_ioc = avg_ioc;
            best_k = k;
        }
        cout << "Key Length: " << k << " \tAverage IOFC: " << avg_ioc << endl;
    }

    cout << "Best key length found: " << best_k << endl;

    string key;

    float mutual;
    float best_mutual = 0;
    float ibest_mutual = 0;
    for (int g = 0; g < best_k; g++) {
        stext = substring(text,best_k,g);
        best_mutual = 0;
        ibest_mutual = 0;
        for (int i = 0; i < 26; i++) {
            mutual = miofc(frequency(stext),shift(getEnglishFrequency(),i));
            if (mutual > best_mutual) {
                best_mutual = mutual;
                ibest_mutual = i;
            }
        }
        key += char(ibest_mutual + 'A');
    }

    cout << "Key found: " << key << endl;
    cout << "Decrypted Text: " << vDecrypt(text,key) << endl;
}

void analyze_affine(const string& text) {
    
    float miofcv;
    float mmiofc = 0;
    int ma = 0, mb = 0;
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

    if (alg != "caesar" && alg != "bcaesar" && alg != "vigenere" && alg != "affine") {
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
        analyze_caesar(text);
    }
    else if (alg == "bcaesar") {
        analyze_caesar_bruteforce(text);
    }
    else if (alg == "vigenere") {
        analyze_vigenere(text);
    }
    else if (alg == "affine") {
        analyze_affine(text);
    }

    return 0;
}