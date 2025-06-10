using namespace std;

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include "../include/cipherutils.h"

using namespace CipherUtils;

void print_usage() {
    std::cout << "Usage:\n"
              << "\ttool gcd <a> <b>\n"
              << "\ttool minverse <a> <mod>\n"
              << "\ttool inverses <a> <b> <mod>\n"
              << "\ttool soc <a> <m> <b> <n>\n"
              << "\ttool totient <n>\n"
              << "\ttool fme <base> <exp> <mod>\n"
              << "\ttool prime <size>\n"
              << "\ttool sgprime <size>\n"
              << "\ttool isprime <n>\n";
}

/*
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

    if (gcd(m,n) != 1) {
        cerr << "Modular values must be coprime"
             << endl;
        return;
    }

    int x = soc(a,m,b,n);

    cout << "System of Congruences:" << endl
         << "x ≡ " + to_string(a) + " (mod " + to_string(m) + ")" << endl
         << "x ≡ " + to_string(b) + " (mod " + to_string(n) + ")" << endl
         << "x = " + to_string(x) << endl;
}

void phi_finder(int n) {
    cout << "φ(" << n << ") = " << totient(n) << endl;
}

void fme_finder(int a, int e, int m) {
    cout << "Result of pow(" << a << "," << e << ") (mod " << m << ") = " << fastModularExponentiation(a, e, m) << " (mod " << m << ")" << endl;
}
*/
void gcd_finder(int a, int b) {
    std::cout << "GCD: " << Arithmetic::gcd(a, b) << std::endl;
}

void fme_solver(std::string a_str, std::string e_str, std::string m_str) {
    mpz_class a(a_str);
    mpz_class e(e_str);
    mpz_class m(m_str);

    std::cout << "Result of Modular Exponentiation:\n" << Exponentiation::largeModularExponentiation(a, e, m).get_str() << std::endl;
}

void prime_checker(std::string number_str) {
    mpz_class number(number_str);
    cout << "Is Prime? " << Primes::isPrime(number) << std::endl;
}

void minverse_finder(std::string n_str, std::string m_str) {
    mpz_class n(n_str);
    mpz_class m(m_str);

    std::cout << "Modular Inverse: " << Arithmetic::modInverse(n, m).get_str() << std::endl;
}

void inverse_checker(std::string a_str, std::string b_str, std::string m_str) {
    mpz_class a(a_str);
    mpz_class b(b_str);
    mpz_class m(m_str);

    mpz_class res = (a * b) % m;
    std::cout << "Result of (a * b) % m: " << res.get_str() << std::endl;
}

void soc_solver(int a, int m, int b, int n) {
    std::cout << "System of Congruence: " << Arithmetic::soc(a,m,b,n) << std::endl;
}

void totient_finder(int n) {
    std::cout << "φ(n): " << Arithmetic::totient(n) << std::endl;
}

void prime_generator(size_t size) {
    std::cout << Primes::generatePrime(size) << std::endl;
}

void sgprime_generator(size_t size) {
    std::cout << Primes::generateSophieGermainPrime(size) << std::endl;
}

void jacobi_solver(std::string a_str, std::string b_str) {
    mpz_class a(a_str);
    mpz_class b(b_str);

    std::cout << "Jacobi: " << NumberTheory::jacobi(a, b) << std::endl;
}

int main(int argc, char** argv) {

    std::set<std::string> valid_tools = {"gcd","minverse","inverses","soc","totient","fme","isprime","prime","sgprime","jacobi"};
    
    if (argc < 2) {
        print_usage();
        return -1;
    }

    string tool = argv[1];

    if (valid_tools.find(tool) == valid_tools.end()) {
        print_usage();
        return -1;
    }

    if (tool == "fme") {
        fme_solver(argv[2], argv[3], argv[4]);
    }
    else if (tool == "isprime") {
        prime_checker(argv[2]);
    }
    else if (tool == "minverse") {
        minverse_finder(argv[2], argv[3]);
    }
    else if (tool == "inverses") {
        inverse_checker(argv[2], argv[3], argv[4]);
    }
    else if (tool == "soc") {
        soc_solver(stoi(argv[2]), stoi(argv[3]), stoi(argv[4]), stoi(argv[5]));
    }
    else if (tool == "totient") {
        totient_finder(stoi(argv[2]));
    }
    else if (tool == "prime") {
        prime_generator(stoi(argv[2]));
    }
    else if (tool == "sgprime") {
        sgprime_generator(stoi(argv[2]));
    }
    else if (tool == "gcd") {
        gcd_finder(stoi(argv[2]), stoi(argv[3]));
    }
    else if (tool == "jacobi") {
        jacobi_solver(argv[2], argv[3]);
    }
    
    return 1;
}