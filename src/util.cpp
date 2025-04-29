using namespace std;

#include <vector>
#include <cmath>
#include <algorithm>
#include <tuple>
#include <utility>
#include "../include/util.h"

// Greatest Common Divisor
// Takes in two integers and returns their greatest common divisor
// Params:
//  a - Integer
//  b - Integer
// Returns:
//  GCD of a and b
int gcd(int a, int b) {
    int t;
    while (b != 0) {
        t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Modular Inverse
// Takes in two integers and returns the modular inverse of the first from the second
// Params:
//  a - Integer
//  m - Integer
// Returns:
//  The modular inverse of a mod m
int modInverse(int a, int m) {
    int m_orig = m;                 // Save original Mod value to account for negative result correction
    int x = 0;                      // Initial Coefficient of m
    int y = 1;                      // Initial Coefficient of a - Coefficients in the formula for GCD: ay' + mx' = g where x = y' = 1 and y = (y' - qx') = 0
    int q;

    while (a > 1) {                 // Continue until GCD of some value and 1
        q = a / m;                  // Find the quotient of the two values
        a = exchange(m, a % m);     // a becomes m, m becomes remainder f(a,m) -> f(m,r)

        // ax + my = g => ay' + m(y'- qx') = g
        // These coefficient terms are the previous and previous previous values
        y = exchange(x, y - q * x); // x becomes y' - q * x', y becomes x'
    }

    y = (y + m_orig) % m_orig;      // Ensure positive result mod m for the second coefficient

    return y;                       // Return Coefficient
}

// Modular Inverse Initial Call
int modInverseRecursive(int a, int m) {
    auto [x,y] = modInverseRecursiveLoop(a, m);
    x = (x + m) % m;    // Ensure positive result mod m for the first coefficient
    return x;
}

// Modular Inverse Recursive
// Takes in two integers and returns the modular inverse of the first from the second
// Params:
//  a - Integer
//  m - Integer
// Returns:
//  The modular inverse of a and m
tuple<int,int> modInverseRecursiveLoop(int a, int m) {

    // Base Case
    if (m == 1) {
        return {0, 1};
    }

    int x, x_p, y, y_p, q, r;

    q = a / m;                                      // Find the quotient of the two values
    r = a % m;                                      // Find the remainder of the two values

    tie(x_p, y_p) = modInverseRecursiveLoop(m, r);  // Retrieve previous x and y values from one level down with recursive call on x and y

    // ax + my = g => ay' + m(y'- qx') = g
    x = y_p;                                        // x becomes y'
    y = x_p - y_p * q;                              // y becomes x' - y' * q

    return {x, y};                                  // Return new coefficients
}

tuple<int,int> affine_key(char p1, char p2, char c1, char c2) {
    int diff = (p1 - p2 + 26) % 26;
    int dinv = modInverse(diff,26);
    int a = (((c1 - c2 + 26) % 26) * dinv) % 26;
    int b = ((c1 - 'a') - (a * (p1 - 'a')) % 26 + 26) % 26;

    return {a, b};
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

bool isNumber(const string& input) {
    for (char c: input) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

// Frequency
// Takes in a string of text and returns a list of frequencies for each charact in the English language
// Params:
//  text - String
// Returns:
//  Frequency list of text for English
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

vector<float> getLanguageFrequency(const string& language) {
    if (language == "English" || language == "english") {
        vector<float> english_freq = {			
            0.082, /* a */
            0.015, /* b */
            0.028, /* c */
            0.043, /* d */ 
            0.127, /* e */
            0.022, /* f */
            0.020, /* g */
            0.061, /* h */
            0.070, /* i */
            0.002, /* j */
            0.008, /* k */
            0.040, /* l */
            0.024, /* m */
            0.067, /* n */
            0.075, /* o */
            0.019, /* p */
            0.001, /* q */
            0.060, /* r */
            0.063, /* s */
            0.091, /* t */
            0.028, /* u */
            0.010, /* v */
            0.023, /* w */
            0.001, /* x */
            0.020, /* y */
            0.001  /* z */
        };

        return english_freq;
    }
    else {
        return {};
    }
}

vector<float> getEnglishFrequency() {
    return getLanguageFrequency("English");
}

tuple<int,int> mostCommon(const vector<float>& freq) {
    int j = 0;
    float max = freq[0];
    int imax = 0;
    int prevIMax = 0;
    for (auto i: freq) {
        if (i > max) {
            prevIMax = imax;
            max = i;
            imax = j;
        }
        j++;
    }
    return {imax, prevIMax};
}

// Index of Coincidence
// Takes in a list of frequencies corresponding to a language and returns it's index of coincidence
// Params:
//  freq - Float Vector
// Returns:
//  The Index of Coincidence of freq
float iofc(const vector<float>& freq) {
    float sum = 0;
    for (auto f: freq) {
        sum += pow(f, 2);
    }
    return sum;
}

// Mutual Index of Coincidence
// Takes in two lists of frequencies and returns their Mutual Index of Coincidence
// Parames:
//  freqA - Float Vector
//  freqB - Float Vector
// Returns
//  Mutual Index of Coincidence of freqA and freqB
float miofc(const vector<float>& freqA, const vector<float>& freqB) {
    float sum = 0;
    for (auto i = 0u; i < freqA.size(); i++) {
        sum += freqA[i] * freqB[i];
    }
    return sum;
}

// Frequency Shift
// Shifts a frequency list, for MIC calculations on shifted ciphertext
// Params:
//  freq - Float Vector
//  shift - Integer
// Returns:
//  Frequency vector freq shifted by shift
vector<float> shift(vector<float> freq, int shift) {
    const size_t n = freq.size();
    std::size_t s = (shift % n + n) % n;
    rotate(freq.begin(), freq.begin() + (n - s), freq.end());
    return freq;
}

// Subtext Group
// Takes in a string of text and returns a substring that contains every letter a specified step distance away, shifted by some amount
// Ex. ABCDEFGHI, Step 3, Shift 1 returns BEH
// Params:
//  str - String
//  step - Integer
//  shift - Integer
// Returns:
//  Substring of str, of characters index minus shift mod step equal to 0
string substring(const string& str, int step, int shift = 0) {
    string res;
    int j = 0;
    for (char c: str) {
        if (isalpha(c)) {
            if ((j - shift) % step == 0) {
                res += toupper(c);
            }
            j++;
        }
    }
    return res;
}

int soc(int a, int m, int b, int n) {
    int n_inv = modInverseRecursive(n, m);
    int m_inv = modInverseRecursive(m, n);
    return (a*n*n_inv + b*m*m_inv) % (m*n);
}