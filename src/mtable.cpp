using namespace std;

#include <iostream>

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <size>"
             << endl;
    }

    int size = stoi(argv[1]);
    int numInverse = 0;

    for (int i = 0; i < size; i ++) {
        for (int j = 0; j < size; j++) {
            cout << (i * j) % size;
            if ((i * j)  % size == 1) {
                numInverse++;
            }
            if ((i * j) % size < 10) {
                cout << ' ';
            }
            if (j != size - 1) {
                cout << ' ';
            }
            else {
                cout << endl;
            }
        }
    }

    cout << "Number of multiplicative inverses: " << numInverse << endl;

    return 0;
}