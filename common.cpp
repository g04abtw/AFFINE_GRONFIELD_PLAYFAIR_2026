
#include "common.h"
#include <iostream>
#include <filesystem>

using namespace std;

string programLanguage = "русский";

bool fileExists(const string& filename) {
    return filesystem::exists(filename);
}

bool createFile(const string& filename, const string& content) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: Невозможно создать файл '" << filename << "'\n";
        return false;
    }

    file << content;
    file.close();

    cout << "Файл '" << filename << "' успешно создан.\n";
    return true;
}

string readFile(const string& filename) {
    if (!fileExists(filename)) {
        throw runtime_error("Файл '" + filename + "' не существует");
    }

    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Невозможно открыть файл '" + filename + "'");
    }

    string content((istreambuf_iterator<char>(file)),
        istreambuf_iterator<char>());
    file.close();

    return content;
}

bool writeToFile(const string& filename, const string& content) {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Ошибка: Невозможно создать файл '" << filename << "'\n";
        return false;
    }

    file << content;
    file.close();

    cout << "Содержимое успешно записано в файл '" << filename << "'\n";
    return true;
}

// Функция для проверки смешанного русско-английского текста с поддержкой спецсимволов
bool validateMixedText(const string& text) {
    for (unsigned char c : text) {
        // Разрешаем все символы от пробела (32) до тильды (126) + русские буквы
        bool isPrintableASCII = (c >= 32 && c <= 126);
        bool isRussian = (c >= 0xC0 && c <= 0xFF) || c == 0xA8 || c == 0xB8;

        if (!(isPrintableASCII || isRussian)) {
            return false;
        }
    }
    return true;
}