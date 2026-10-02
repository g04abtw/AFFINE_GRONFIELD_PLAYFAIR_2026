#include <iostream>
#include <string>
#include <limits>
#include <random>
#include "AffineCipher.h"
#include "common.h"
using namespace std;

class AffineCipherCore {
private:
    const int ALPHABET_SIZE = 256;
    int a, b;

    int gcd(int x, int y) { //НОД
        while (y != 0) {
            int temp = y;
            y = x % y;
            x = temp;
        }
        return x;
    }

    int modInverse(int a, int m) {
        a = a % m;
        for (int x = 1; x < m; x++) {
            if ((a * x) % m == 1) {
                return x;
            }
        }
        return -1;
    }

    bool isValidKey(int a, int m) {
        return gcd(a, m) == 1;
    }

public:
    bool setKeys(int key_a, int key_b) {
        if (isValidKey(key_a, ALPHABET_SIZE)) {
            a = key_a;
            b = key_b;
            return true;
        }
        return false;
    }

    string encrypt(const string& plaintext) {
        string ciphertext;
        for (unsigned char c : plaintext) {
            ciphertext += static_cast<unsigned char>((a * static_cast<int>(c) + b) % ALPHABET_SIZE);
        }
        return ciphertext;
    }

    string decrypt(const string& ciphertext) {
        string plaintext;
        int a_inv = modInverse(a, ALPHABET_SIZE);
        if (a_inv == -1) {
            throw runtime_error("Невозможно найти обратный элемент для ключа 'a'");
        }

        for (unsigned char c : ciphertext) {
            int result = a_inv * (static_cast<int>(c) - b);
            while (result < 0) {
                result += ALPHABET_SIZE;
            }
            plaintext += static_cast<unsigned char>(result % ALPHABET_SIZE);
        }
        return plaintext;
    }
};

bool validateTextAffine(const string& text, const string& language) {
    return validateMixedText(text);
}

string encryptAffineCipher(const string& text, int a, int b) {
    AffineCipherCore cipher;
    if (!cipher.setKeys(a, b)) {
        throw runtime_error("Неверные ключи для аффинного шифра");
    }
    return cipher.encrypt(text);
}

string decryptAffineCipher(const string& text, int a, int b) {
    AffineCipherCore cipher;
    if (!cipher.setKeys(a, b)) {
        throw runtime_error("Неверные ключи для аффинного шифра");
    }
    return cipher.decrypt(text);
}

void runAffineCipher() {
    string text, inputFilename, outputFilename;
    int a, b, action;
    char useFile;

    cout << "АФИННЫЙ ШИФР (ALPHABET_SIZE = 256)\n";

    cout << "Читать из файла? (y/n): ";
    cin >> useFile;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (useFile == 'y' || useFile == 'Y') {
        cout << "Введите имя входного файла: ";
        getline(cin, inputFilename);

        if (!fileExists(inputFilename)) {
            char createNew;
            cout << "Файл '" + inputFilename + "' не существует. Создать новый файл? (y/n): ";
            cin >> createNew;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (createNew == 'y' || createNew == 'Y') {
                string fileContent;
                cout << "Введите содержимое для нового файла: ";
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
                cout << "Текст успешно прочитан из файла (" << text.length() << " символов).\n";
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

    cout << "Введите ключи a и b (через пробел): ";
    cin >> a >> b;

    try {
        string result;
        if (action == 1) {
            result = encryptAffineCipher(text, a, b);
            cout << "Зашифрованный текст: " << result << endl;
        }
        else {
            result = decryptAffineCipher(text, a, b);
            cout << "Расшифрованный текст: " << result << endl;
        }

        cout << "Сохранить в файл? (y/n): ";
        cin >> useFile;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (useFile == 'y' || useFile == 'Y') {
            cout << "Введите имя выходного файла: ";
            getline(cin, outputFilename);

            if (writeToFile(outputFilename, result)) {
                cout << "Результат сохранен в файл: " << outputFilename << endl;
            }
        }
    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}