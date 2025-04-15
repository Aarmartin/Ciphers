using namespace std;

#include <vector>
#include <cmath>
#include <algorithm>
#include "../include/util.h"

int gcd(int a, int b) {
    int t;
    while (b != 0) {
        t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int modInverse(int a, int m) {
    int m0 = m;
    int x0 = 0;
    int x1 = 1;
    int q, t;

    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;

        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    x1 = (x1 + m0) % m0;

    return x1;
}

string strToUpper(const string& str) {
    string result;
    for (char c: str) {
        result += toupper(c);
    }
    return result;
}

string strToLower(const string& str) {
    string result;
    for (char c: str) {
        result += tolower(c);
    }
    return result;
}

vector<float> frequency(const string& text) {
    vector<int> count(26,0);
    vector<float> freq(26);
    int sum = 0;
    for (char c: text) {
        if (isalpha(c)) {
            c = toupper(c);
            count[c - 'A']++;
            sum++;
        }
    }
    for (auto i = 0u; i < freq.size(); i++) {
        freq[i] = float(count[i]) / sum;
    }
    return freq;
}

float iofc(const vector<float>& freq) {
    float sum = 0;
    for (auto f: freq) {
        sum += pow(f, 2);
    }
    return sum;
}

float miofc(const vector<float>& freqA, const vector<float>& freqB) {
    float sum = 0;
    for (auto i = 0u; i < freqA.size(); i++) {
        sum += freqA[i] * freqB[i];
    }
    return sum;
}

vector<float> shift(vector<float> freq, int shift) {
    const size_t n = freq.size();
    std::size_t s = (shift % n + n) % n;
    rotate(freq.begin(), freq.begin() + (n - s), freq.end());
    return freq;
}

string substring(const string& str, int step, int shift = 0) {
    string res;
    int j = 0;
    for (char c: str) {
        if (isalpha(c)) {
            if ((j + shift) % step == 0) {
                res += toupper(c);
            }
            j++;
        }
    }
    return res;
}