
#ifndef PLAYFAIR_H
#define PLAYFAIR_H

#include <string>

bool validateTextPlayfair(const std::string& text, const std::string& language);
std::string encryptPlayfairCipher(const std::string& text, const std::string& key);
std::string decryptPlayfairCipher(const std::string& text, const std::string& key);
void runPlayfairCipher();

#endif