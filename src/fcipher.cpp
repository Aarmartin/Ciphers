using namespace std;

#include <vector>

using Int = int;
using Vec = vector<int>;
using Mat = vector<Vec>;

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <gmpxx.h>
#include "../include/ciphers.h"
#include "../include/util.h"

void main_lwe_keygen(const string &fname_pk, const string &fname_sk);
void main_lwe_encrypt(const string &fname_pk, const string &text, const string &fname);
void main_lwe_decrypt(const string &fname_sk, const string &text, const string &fname);

void savePubKey(const PublicKey &pk, const string &fname) {
    ofstream f(fname, ios::binary);
    int n = pk.A.size(), m = pk.b.size();
    f.write((char*)&n, sizeof n);
    f.write((char*)&m, sizeof m);
    f.write((char*)&pk.b[0], sizeof(Int)*m);
    for (auto &row : pk.A)
        f.write((char*)&row[0], sizeof(Int)*m);
}

PublicKey loadPubKey(const string &fname) {
    ifstream f(fname, ios::binary);
    int n, m; f.read((char*)&n, sizeof n);
    f.read((char*)&m, sizeof m);
    PublicKey pk; pk.A.assign(n, Vec(m)); pk.b.resize(m);
    f.read((char*)&pk.b[0], sizeof(Int)*m);
    for (auto &row : pk.A)
        f.read((char*)&row[0], sizeof(Int)*m);
    return pk;
}

void savePrivKey(const PrivateKey &sk, const string &fname) {
    ofstream f(fname, ios::binary);
    int n = sk.s.size();
    f.write(reinterpret_cast<const char*>(&n), sizeof(n));
    f.write(reinterpret_cast<const char*>(sk.s.data()), sizeof(Int)*n);
}

PrivateKey loadPrivKey(const string &fname) {
    ifstream f(fname, ios::binary);
    int n; f.read(reinterpret_cast<char*>(&n), sizeof(n));
    PrivateKey sk; sk.s.resize(n);
    f.read(reinterpret_cast<char*>(sk.s.data()), sizeof(Int)*n);
    return sk;
}

void main_lwe(const string &op, const string &text, const string &fname) {
    if (op == "keygen") {
        main_lwe_keygen(fname + ".pk", fname + ".sk");
    }
    else if (op == "encrypt") {
        string kname;
        cout << "Public Key Filename: "; cin >> kname;
        main_lwe_encrypt(kname, text, fname);
    }
    else if (op == "decrypt") {
        string kname;
        cout << "Private Key Filename: "; cin >> kname;
        main_lwe_decrypt(kname, text, fname);
    }
}

void main_lwe_keygen(const string &fname_pk, const string &fname_sk) {
    int n, m; Int q; double sigma;
    //cout << "n m q sigma? "; cin >> n >> m >> q >> sigma;
    n = 512; m = 1024; q = 4093; sigma = 3.19;
    LWE engine(n,m,q,sigma);
    PublicKey pk; PrivateKey sk;
    engine.keygen(pk, sk);
    savePubKey(pk, fname_pk);
    savePrivKey(sk, fname_sk);
    cout << "Wrote: " << fname_pk << " and " << fname_sk << endl;
}

void main_lwe_encrypt(const string &fname_pk, const string &text, const string &fname) {
    PublicKey pk = loadPubKey(fname_pk);

    LWE engine(512, 1024, 4093, 3.19);
    vector<CipherText> cts;
    int mu;
    for (auto &bit : text)
        if (bit == '0' || bit == '1') {
            mu = bit - '0';
            cts.push_back(engine.encrypt(pk, mu));
        }
    ofstream fout(fname + ".enc", ios::binary);
    int count = (int)cts.size();
    fout.write(reinterpret_cast<char*>(&count),sizeof(count));
    for (auto &ct : cts) {
        fout.write(
            reinterpret_cast<char*>(ct.c1.data()),
            sizeof(Int) * ct.c1.size()
        );

        fout.write(
            reinterpret_cast<char*>(&ct.c2[0]),
            sizeof(Int)
        );
    }

    cout << "Encrypted: " << count << " bits -> " << fname << ".enc" << endl;
}

void main_lwe_decrypt(const string &fname_sk, const string &text, const string &fname) {
    PrivateKey sk = loadPrivKey(fname_sk);
    cout << "Private key file name: " << fname_sk << endl;

    LWE engine(512, 1024, 4093, 3.19);

    ifstream fin(fname, ios::binary);
    if (!fin) {
        cerr << "File not found"
             << endl;
        return;
    }

    int count;
    fin.read(reinterpret_cast<char*>(&count), sizeof(count));
    string recovered;
    recovered.reserve(count);

    for (int i = 0; i < count; i++) {
        Vec c1(sk.s.size());
        fin.read(reinterpret_cast<char*>(c1.data()),
            sizeof(Int) * c1.size());

        Int c2;
        fin.read(reinterpret_cast<char*>(&c2), sizeof(c2));

        CipherText ct{ c1, Vec {c2}};

        int bit = engine.decrypt(sk, ct);
        bit = bit + '0';

        recovered.push_back(char('0' + bit));
    }

    ofstream fout(fname + ".dec");
    fout << recovered;
    cout << "Decrypted: " << fname + ".dec" << endl;
}

void main_caesar(const string& op, const string& text, const string& fname) {

    int key;
    bool enc = op == "encrypt";

    cout << "Enter your key: ";
    cin >> key;
    cin.ignore();

    if (enc) {
        string ciphertext = cEncrypt(text,key);
        string oname = filesystem::path(fname).replace_extension(".enc").string();
        ofstream of(oname);
        if (!of) {
            cerr << "Could not open output file" << endl;
        }
        of << ciphertext;
        of.close();
    }
    else {
        string plaintext = cDecrypt(text,key);
        string oname = filesystem::path(fname).replace_extension(".dec").string();
        ofstream of(oname);
        if (!of) {
            cerr << "Could not open output file" << endl;
        }
        of << plaintext;
        of.close();
    }

}

void main_vigenere(const string& op, const string& text, const string& fname) {

    string key;
    bool enc = op == "encrypt";

    cout << "Enter your key: ";
    getline(cin,key);

    if (enc) {
        string ciphertext = vEncrypt(text,key);
        string oname = filesystem::path(fname).replace_extension(".enc").string();
        ofstream of(oname);
        if (!of) {
            cerr << "Could not open output file" << endl;
        }
        of << ciphertext;
        of.close();
    }
    else {
        string plaintext = vDecrypt(text,key);
        string oname = filesystem::path(fname).replace_extension(".dec").string();
        ofstream of(oname);
        if (!of) {
            cerr << "Could not open output file" << endl;
        }
        of << plaintext;
        of.close();
    }
}

void main_affine(const string& op, const string& text, const string& fname) {
    string key;
    int a;
    int b;
    bool enc = op == "encrypt";

    cout << "Enter your key a,b: ";
    cin >> key;
    cin.ignore();

    a = stoi(key.substr(0,key.find(',')));
    b = stoi(key.substr(key.find(',') + 1));

    if (gcd(a,26) != 1) {
        cout << "Enter a valid key (First value must be coprime with 26)"
             << endl;
        return;
    }

    if (enc) {
        string ciphertext = aEncrypt(text,a,b);
        string oname = filesystem::path(fname).replace_extension(".enc").string();
        ofstream of(oname);
        if (!of) {
            cerr << "Could not open output file" << endl;
        }
        of << ciphertext;
        of.close();
    }
    else {
        string plaintext = aDecrypt(text,a,b);
        string oname = filesystem::path(fname).replace_extension(".dec").string();
        ofstream of(oname);
        if (!of) {
            cerr << "Could not open output file" << endl;
        }
        of << plaintext;
        of.close();
    }
}

void main_rsa(const string& op, const string& text, const string& fname) {
    if (op == "encrypt") {
        std::string str_n;
        std::string str_e;

        cout << "Enter value for n: ";
        cin >> str_n;
        cin.ignore();
        mpz_class n(str_n);

        cout << "Enter value for e: ";
        cin >> str_e;
        cin.ignore();
        mpz_class e(str_e);
        
        std::vector<mpz_class> ciphertext = rEncrypt(text, n, e);

        string oname = filesystem::path(fname).replace_extension(".enc").string();
        ofstream of(oname);
        
        for (auto &c : ciphertext) {
            of << c.get_str(10) << "\n";
        }

        of.close();
    }
    else {
        std::string str_n;
        std::string str_d;

        cout << "Enter value for n: ";
        cin >> str_n;
        cin.ignore();
        mpz_class n(str_n);

        cout << "Enter value for d: ";
        cin >> str_d;
        cin.ignore();
        mpz_class d(str_d);

        ifstream f(fname);
        std::vector<mpz_class> input;
        std::string line;
        while (std::getline(f,line)) {
            if (line.empty()) continue;
            input.emplace_back(line, 10);
        }

        std::string plaintext = rDecrypt(input, n, d);
        while (!plaintext.empty() && plaintext.back() == '\0') {
            plaintext.pop_back();
        }
        string oname = filesystem::path(fname).replace_extension(".dec").string();
        ofstream of(oname);
        if (!of) {
            cerr << "Could not open output file" << endl;
        }
        of << plaintext;
        of.close();
    }
}

int main(int argc, char** argv){

    if (argc != 4) {
        cerr << "Usage:" << argv[0]
             << " <encryption algorithm> <[encrypt|decrypt|keygen]> <filename>"
             << endl;
        return 1;
    }

    string alg = argv[1];
    string op = argv[2];
    string fname = argv[3];

    if (alg != "caesar" && alg != "vigenere" && alg != "affine" && alg != "lattice" && alg != "rsa") {
        cerr << "Enter a valid encryption method. (caesar, vigenere, affine, rsa)"
             << endl;
        return 1;
    }

    if (op != "encrypt" && op != "decrypt" && op != "keygen") {
        cerr << "Enter a valid operation. (encrypt, decrypt)"
             << endl;
        return 1;
    }
    
    string line;
    string text;
    ifstream f;

    if (op != "keygen") {
        f.open(fname);
        if (!f.is_open()) {
            cout << "File not found"
                << endl;
            return 1;
        }
        while (getline(f,line)) {
            if (!f.eof()) {
                line += ' ';
            }
            text += line;
        }
    }

    if (alg == "caesar") {
        main_caesar(op, text, fname);
    }
    else if (alg == "vigenere") {
        main_vigenere(op, text, fname);
    }
    else if (alg == "affine") {
        main_affine(op, text, fname);
    }
    else if (alg == "lattice") {
        main_lwe(op, text, fname);
    }
    else if (alg == "rsa") {
        main_rsa(op, text, fname);
    }

    return 0;
}

