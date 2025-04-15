using namespace std;

#include <iostream>
#include <fstream>
#include <vector>
#include "../include/util.h"
#include "../include/caesar.h"

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