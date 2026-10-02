#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "common.h"

using namespace std;

class GronsfeldCipherCore {
private:
    const int ALPHABET_SIZE = 256;
    vector<int> key;

public:
    bool setKey(const string& keyStr) {
        key.clear();
        for (char c : keyStr) {
            if (!isdigit(c)) return false;
            key.push_back(c - '0');
        }
        return !key.empty();
    }

    string encrypt(const string& plaintext) {
        string ciphertext;
        ciphertext.reserve(plaintext.size());

        for (size_t i = 0; i < plaintext.size(); i++) {
            int shift = key[i % key.size()];
            unsigned char c = plaintext[i];
            ciphertext += static_cast<unsigned char>((c + shift) % ALPHABET_SIZE);
        }
        return ciphertext;
    }

    string decrypt(const string& ciphertext) {
        string plaintext;
        plaintext.reserve(ciphertext.size());

        for (size_t i = 0; i < ciphertext.size(); i++) {
            int shift = key[i % key.size()];
            unsigned char c = ciphertext[i];
            int value = (c - shift) % ALPHABET_SIZE;
            if (value < 0) value += ALPHABET_SIZE;
            plaintext += static_cast<unsigned char>(value);
        }
        return plaintext;
    }
};

bool validateTextGronsfeld(const string& text, const string&) {
    return validateMixedText(text);
}

string encryptGronsfeldCipher(const string& text, const string& key) {
    GronsfeldCipherCore cipher;
    if (!cipher.setKey(key)) {
        throw runtime_error("Ключ должен состоять только из цифр");
    }
    return cipher.encrypt(text);
}

string decryptGronsfeldCipher(const string& text, const string& key) {
    GronsfeldCipherCore cipher;
    if (!cipher.setKey(key)) {
        throw runtime_error("Ключ должен состоять только из цифр");
    }
    return cipher.decrypt(text);
}

void runGronsfeldCipher() {
    string text, inputFilename, outputFilename, key;
    int action;
    char useFile;

    cout << "ШИФР ГРОНСФЕЛЬДА (ALPHABET_SIZE = 256)\n";

    cout << "Читать из файла? (y/n): ";
    cin >> useFile;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (useFile == 'y' || useFile == 'Y') {
        cout << "Введите имя входного файла: ";
        getline(cin, inputFilename);

        if (!fileExists(inputFilename)) {
            char createNew;
            cout << "Файл '" + inputFilename + "' не существует. Создать новый? (y/n): ";
            cin >> createNew;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (createNew == 'y' || createNew == 'Y') {
                string fileContent;
                cout << "Введите содержимое: ";
                getline(cin, fileContent);

                if (!createFile(inputFilename, fileContent)) {
                    return;
                }
                text = fileContent;
            }
            else {
                return;
            }
        }
        else {
            try {
                text = readFile(inputFilename);
                cout << "Текст успешно прочитан (" << text.length() << " символов).\n";
            }
            catch (const exception& e) {
                cerr << "Ошибка чтения файла: " << e.what() << endl;
                return;
            }
        }
    }
    else {
        cout << "Введите текст: ";
        getline(cin, text);
    }

    cout << "Выберите действие: 1 - Шифрование, 2 - Дешифрование: ";
    cin >> action;

    cout << "Введите цифровой ключ (например '12345'): ";
    cin >> key;

    try {
        string result;

        if (action == 1) {
            result = encryptGronsfeldCipher(text, key);
            cout << "Зашифрованный текст: " << result << endl;
        }
        else {
            result = decryptGronsfeldCipher(text, key);
            cout << "Расшифрованный текст: " << result << endl;
        }

        cout << "Сохранить в файл? (y/n): ";
        cin >> useFile;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (useFile == 'y' || useFile == 'Y') {
            cout << "Введите имя выходного файла: ";
            getline(cin, outputFilename);

            if (writeToFile(outputFilename, result)) {
                cout << "Результат сохранён в файл: " << outputFilename << endl;
            }
        }
    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}
