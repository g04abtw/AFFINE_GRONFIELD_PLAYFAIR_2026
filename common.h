
#pragma once
#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <fstream>
#include <stdexcept>

std::string readFile(const std::string& filename);
bool writeToFile(const std::string& filename, const std::string& content);
bool createFile(const std::string& filename, const std::string& content = "");
bool fileExists(const std::string& filename);

bool validateMixedText(const std::string& text);

extern std::string programLanguage;

#endif