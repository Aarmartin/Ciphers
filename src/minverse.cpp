using namespace std;

#include <iostream>
#include "../include/util.h"

int main(int argc, char** argv) {
    
    if (argc != 3 || !isNumber(argv[1]) || !isNumber(argv[2])) {
        cout << "Usage: " << argv[0] << "<number: int> <mod: int>"
             << endl;
        return 1;
    }

    int n = stoi(argv[1]);
    int m = stoi(argv[2]);

    cout << "Mod Inverse of " << n << " mod " << m << ": " << modInverse(n,m)
         << endl;

    return 1;
}