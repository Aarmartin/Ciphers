#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

int gcd(int a, int b);

int modInverse(int a, int m);

std::string strToUpper(const std::string& str);

std::string strToLower(const std::string& str);

std::vector<float> frequency(const std::string& text);

float iofc(const std::vector<float>& freq);

float miofc(const std::vector<float>& freqA, const std::vector<float>& freqB);

std::vector<float> shift(std::vector<float> freq, int shift);

std::string substring(const std::string& str, int step, int shift);

#endif