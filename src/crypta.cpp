using namespace std;

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include "../include/util.h"
#include "../include/ciphers.h"

// Caesar Cryptanalysis function
void analyze_caesar(const string& text) {
    
    vector<float> english_freq = getEnglishFrequency();

    int imax = 0;
    float max = 0;
    vector<float> freq = frequency(text);

    // Loop through each possible shift
    for (int i = 0; i < 26; i++) {

        // Compare Mutual Index of Coincidence of ciphertext with shifted english frequency
        float mutual = miofc(freq,shift(english_freq,i));
        cout << "Mutual found for " << i << ": " << mutual << endl;

        // Track best MIC
        if (mutual > max) {
            max = mutual;
            imax = i;
        }
    }

    // Decrypt
    cout << "Index Found: " << imax << endl;
    cout << "Decrypted Text: " << cDecrypt(text,imax) << endl;
}

// Caesar Cryptanalysis funcion through bruteforce
void analyze_caesar_bruteforce(const string& text) {
    for (int i = 1; i < 26; i++) {
        cout << "Decrypted with Key " << i << ": " << cDecrypt(text,i) << endl;
    }
}

// Vigenere Cryptanalysis function
void analyze_vigenere(const string& text) {

    // Find Friendman approximation of Vigenere key length
    int n = text.length();
    float ic = iofc(frequency(text));                                           // Index of Coincidence
    int fk = int(((0.0265 * n) / ((ic * (n-1)) - 0.0385 * n + 0.065)) + 0.5f);  // Friedman formula

    cout << "Suspected Friedman Key Length: " << fk << " Adjusted: " << fk + int(sqrt(fk) + 4) << endl;

    vector<float> freq;
    float ioc;
    float best_ioc = 0;
    int best_k = 0;
    float avg_ioc;
    string stext;

    // Loop through each possible key length up to Friedman key length plus a small amount
    for (int k = 2; k < fk + int(sqrt(fk) + 4); k++) {
        avg_ioc = 0;

        // Loop through each subtext group
        // Ex. ABCDEFGHI, Key length 3, Loop 1: ADG, Loop 2: BEH, Loop 3: CFI
        for (int s = 0; s < k; s++) {
            stext = substring(text,k,s);
            freq = frequency(stext);

            // Calculate Index of Coincidence
            ioc = iofc(freq);
            avg_ioc += ioc;
        }
        
        // Take average IC
        avg_ioc = avg_ioc / k;

        // Track best IC for each key length
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

    // Loop through each subtext group for best key length found
    for (int g = 0; g < best_k; g++) {
        stext = substring(text,best_k,g);
        best_mutual = 0;
        ibest_mutual = 0;

        // Perform Caesar Cryptanalysis on subtext
        for (int i = 0; i < 26; i++) {
            mutual = miofc(frequency(stext),shift(getEnglishFrequency(),i));
            if (mutual > best_mutual) {
                best_mutual = mutual;
                ibest_mutual = i;
            }
        }

        // Add Character to Key
        key += char(ibest_mutual + 'A');
    }

    // Decrypt
    cout << "Key found: " << key << endl;
    cout << "Decrypted Text: " << vDecrypt(text,key) << endl;
}

// Affine Cryptanalysis Function
void analyze_affine(const string& text) {
    
    float miofcv;
    float mmiofc = 0;
    int ma = 0, mb = 0;
    string plaintext;

    // Loop through each possible value for b in affine key (a,b)
    for (int i = 0; i < 26; i++) {
        // Loop through each possible value of a, where a mod 26 has a GCD of 1 (coprime)
        for (int j = 0; j < 26; j++) {
            if (gcd(j,26) == 1) {
                //Decrypt Plaintext
                plaintext = aDecrypt(text,j,i);

                // Calculate Mutual Index of Coincidence
                miofcv = miofc(frequency(plaintext),getEnglishFrequency());

                // Track best MIC
                if (miofcv > mmiofc) {
                    mmiofc = miofcv;
                    ma = j;
                    mb = i;
                }
            }
        }
    }

    // Decrypt
    cout << "Key: (" << ma << "," << mb << ")" << endl
         << "Decrypted Text: " << aDecrypt(text,ma,mb) << endl;
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