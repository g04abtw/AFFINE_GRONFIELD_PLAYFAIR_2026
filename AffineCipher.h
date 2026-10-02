#ifndef AFFINE_H
#define AFFINE_H

#include <string>

bool validateTextAffine(const std::string& text, const std::string& language);
std::string encryptAffineCipher(const std::string& text, int a, int b);
std::string decryptAffineCipher(const std::string& text, int a, int b);
void runAffineCipher();

#endif