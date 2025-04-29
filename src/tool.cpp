using namespace std;

#include <iostream>
#include <fstream>
#include <vector>
#include "../include/util.h"

void affine_key_finder() {
    string plain;
    string cipher;
    cout << "Input plaintext (2 Letters): ";
    cin >> plain;
    cin.ignore();
    cout << "Input ciphertext: ";
    cin >> cipher;
    cin.ignore();

    if (plain.length() < 2 || cipher.length() < 2) {
        cerr << "Insufficient text length" << endl;
        return;
    }

    auto [a,b] = affine_key(plain[0],plain[1],cipher[0],cipher[1]);

    cout << "Affine Key: (" << a << "," << b << ")"
         << endl;
}

void mod_inverse(int n, int m) {
    cout << "Mod Inverse of " << n << " mod " << m << ": " << modInverseRecursive(n,m)
         << endl;
}

void mod_table(int m) {
    int numInverse = 0;

    for (int i = 0; i < m; i ++) {
        for (int j = 0; j < m; j++) {
            cout << (i * j) % m;
            if ((i * j)  % m == 1) {
                numInverse++;
            }
            if ((i * j) % m < 10) {
                cout << ' ';
            }
            if (j != m - 1) {
                cout << ' ';
            }
            else {
                cout << endl;
            }
        }
    }

    cout << "Number of multiplicative inverses: " << numInverse << endl;
}

void frequency_finder(const string& filename) {
    ifstream f;
    f.open(filename);

    if (!f.is_open()) {
        cout << "File not found"
             << endl;
        return;
    }

    string line;
    string text;

    while (getline(f,line)) {
        if (!f.eof()) {
            line += ' ';
        }
        text += line;
    }

    cout << text << endl;

    vector<float> freq = frequency(text);
    float indxOfCoic = iofc(freq);
    cout << "Index of Coincidence: " << indxOfCoic << endl;

    for (auto i = 0u; i < freq.size(); i++) {
        cout << char(i + 'a') << ':' << freq[i];
        if (i < freq.size() - 1) {
            cout << ", ";
        }
    }
    cout << endl;
}

void soc_solver(int a, int m, int b, int n) {
    int x = soc(a,m,b,n);

    cout << "System of Congruences:" << endl
         << "x ≡ " + to_string(a) + " (mod " + to_string(m) + ")" << endl
         << "x ≡ " + to_string(b) + " (mod " + to_string(n) + ")" << endl
         << "x = " + to_string(x) << endl;
}

int main(int argc, char** argv) {
    
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <findkey|minverse|mtable|frequency|soc> [arguments]"
             << endl;
        return 1;
    }

    string tool = argv[1];

    if (tool != "findkey" && tool != "minverse" && tool != "mtable" && tool !="frequency" && tool != "soc") {
        cerr << "Usage: " << argv[0] << " <findkey|minverse|mtable|frequency|soc> [arguments]"
             << endl;
        return 1;
    }

    if (tool == "findkey") {
        affine_key_finder();
    }
    else if (tool == "minverse") {
        if (argc != 4 || !isNumber(argv[2]) || !isNumber(argv[3])) {
            cout << "Usage: " << argv[0] << " " << argv[1] << " <int> <int>"
                 << endl;
            return 1;
        }
        mod_inverse(stoi(argv[2]),stoi(argv[3]));
    }
    else if (tool == "mtable") {
        if (argc != 3 || !isNumber(argv[2])) {
            cout << "Usage: " << argv[0] << " " << argv[1] << " <int>"
                 << endl;
            return 1;
        }
        mod_table(stoi(argv[2]));
    }
    else if (tool == "frequency") {
        if (argc != 3) {
            cout << "Usage: " << argv[0] << " " << argv[1] << " <filename>"
                 << endl;
            return 1;
        }
        frequency_finder(argv[2]);
    }
    else if (tool == "soc") {
        if (argc != 6) {
            cout << "Usage: " << argv[0] << " " << argv[1] << " <x_1> <n_1> <x_2> <n_2>"
                 << endl;
            return 1;
        }
        soc_solver(stoi(argv[2]),stoi(argv[3]),stoi(argv[4]),stoi(argv[5]));
    }

    return 1;
}