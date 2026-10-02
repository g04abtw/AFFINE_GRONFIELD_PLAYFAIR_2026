#pragma once
#ifndef GRONSFELD_CIPHER_H
#define GRONSFELD_CIPHER_H

#include <string>

bool validateTextGronsfeld(const std::string& text, const std::string& language);

std::string encryptGronsfeldCipher(const std::string& text, const std::string& key);
std::string decryptGronsfeldCipher(const std::string& text, const std::string& key);

// Интерфейс для меню
void runGronsfeldCipher();

#endif
