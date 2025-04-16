using namespace std;

#include <iostream>
#include "../include/util.h"

int main(int argc, char** argv) {
    
    if (argc != 5) {
        cout << "Usage: " << argv[0] << " <plaintext char 1>, <plaintext char 2>, <cipher char 1>, <cipher char 2>"
             << endl;
        return 1;
    }

    char p1 = argv[1][0];
    char p2 = argv[2][0];
    char c1 = argv[3][0];
    char c2 = argv[4][0];

    auto [a, b] = affine_key(p1,p2,c1,c2);

    cout << "Affine Key: (" << a << "," << b << ")"
         << endl;

    return 1;
}