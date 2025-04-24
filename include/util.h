#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>
#include <tuple>

int gcd(int a, int b);

int modInverse(int a, int m);

int modInverseRecursive(int a, int m);

tuple<int,int> modInverseRecursiveLoop(int a, int m);

std::tuple<int,int> affine_key(char p1, char p2, char c1, char c2);

std::string strToUpper(const std::string& str);

std::string strToLower(const std::string& str);

bool isNumber(const std::string& input);

std::vector<float> frequency(const std::string& text);

std::vector<float> getLanguageFrequency(const std::string& language);

std::vector<float> getEnglishFrequency();

std::tuple<int,int> mostCommon(const std::vector<float>& freq);

float iofc(const std::vector<float>& freq);

float miofc(const std::vector<float>& freqA, const std::vector<float>& freqB);

std::vector<float> shift(std::vector<float> freq, int shift);

std::string substring(const std::string& str, int step, int shift);

#endif