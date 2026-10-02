#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <limits>
#include <fstream>
#include "PlayfairCipher.h"
#include "common.h"
using namespace std;

class PlayfairCipherCore {
private:
    const int SIZE = 16; 
    unsigned char matrix[16][16];
    string key;

    const unsigned char PLACEHOLDER = 255;

    // Подготовка ключа - удаление дубликатов
    string prepareKey(const string& inputKey) {
        string preparedKey;
        bool used[256] = { false };

        for (unsigned char c : inputKey) {
            if (!used[static_cast<unsigned char>(c)]) {
                preparedKey += c;
                used[static_cast<unsigned char>(c)] = true;
            }
        }

        for (int i = 0; i < 256; i++) {
            if (!used[i]) {
                preparedKey += static_cast<unsigned char>(i);
            }
        }

        return preparedKey;
    }

    // Создание матрицы 16x16 из ключа
    void createMatrix(const string& preparedKey) {
        int index = 0;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                matrix[i][j] = static_cast<unsigned char>(preparedKey[index++]);
            }
        }
    }

    // Поиск позиции символа в матрице
    pair<int, int> findPosition(unsigned char ch) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (matrix[i][j] == ch) {
                    return make_pair(i, j);
                }
            }
        }
        return make_pair(-1, -1); 
    }

    string prepareTextForEncryption(const string& text) {
        string result;

        for (size_t i = 0; i < text.length(); i++) {
            unsigned char current = static_cast<unsigned char>(text[i]);
            result += static_cast<char>(current);

            if (i + 1 < text.length()) {
                unsigned char next = static_cast<unsigned char>(text[i + 1]);
                if (current == next) {
                    result += static_cast<char>(PLACEHOLDER);
                }
            }
        }

        if (result.length() % 2 != 0) {
            result += static_cast<char>(PLACEHOLDER);
        }

        return result;
    }

    // Подготовка текста для дешифрования (без модификаций)
    string prepareTextForDecryption(const string& text) {
        if (text.length() % 2 != 0) {
            throw runtime_error("Текст для дешифрования должен иметь четную длину");
        }
        return text;
    }

    // Восстановление оригинального текста после дешифрования
    string restoreText(const string& decrypted) {
        string result;

        for (size_t i = 0; i < decrypted.length(); i++) {
            unsigned char current = static_cast<unsigned char>(decrypted[i]);

            if (current != PLACEHOLDER) {
                result += static_cast<char>(current);
            }
            else {
                if (i > 0 && i + 1 < decrypted.length()) {
                    unsigned char prev = static_cast<unsigned char>(decrypted[i - 1]);
                    unsigned char next = static_cast<unsigned char>(decrypted[i + 1]);

                    if (prev == next) {
                        continue;
                    }
                }
                result += static_cast<char>(current);
            }
        }

        return result;
    }

public:
    PlayfairCipherCore(const string& inputKey) {
        key = prepareKey(inputKey);
        createMatrix(key);
    }

    string encrypt(const string& plaintext) {
        string preparedText = prepareTextForEncryption(plaintext);
        string ciphertext;

        for (size_t i = 0; i < preparedText.length(); i += 2) {
            unsigned char first = static_cast<unsigned char>(preparedText[i]);
            unsigned char second = static_cast<unsigned char>(preparedText[i + 1]);

            auto pos1 = findPosition(first);
            auto pos2 = findPosition(second);

            int row1 = pos1.first, col1 = pos1.second;
            int row2 = pos2.first, col2 = pos2.second;

            if (row1 == row2) {
                ciphertext += static_cast<char>(matrix[row1][(col1 + 1) % SIZE]);
                ciphertext += static_cast<char>(matrix[row2][(col2 + 1) % SIZE]);
            }
            else if (col1 == col2) {
                ciphertext += static_cast<char>(matrix[(row1 + 1) % SIZE][col1]);
                ciphertext += static_cast<char>(matrix[(row2 + 1) % SIZE][col2]);
            }
            else {
                ciphertext += static_cast<char>(matrix[row1][col2]);
                ciphertext += static_cast<char>(matrix[row2][col1]);
            }
        }

        return ciphertext;
    }

    string decrypt(const string& ciphertext) {
        string preparedText = prepareTextForDecryption(ciphertext);
        string decrypted;

        for (size_t i = 0; i < preparedText.length(); i += 2) {
            unsigned char first = static_cast<unsigned char>(preparedText[i]);
            unsigned char second = static_cast<unsigned char>(preparedText[i + 1]);

            auto pos1 = findPosition(first);
            auto pos2 = findPosition(second);

            int row1 = pos1.first, col1 = pos1.second;
            int row2 = pos2.first, col2 = pos2.second;

            if (row1 == row2) {
                decrypted += static_cast<char>(matrix[row1][(col1 - 1 + SIZE) % SIZE]);
                decrypted += static_cast<char>(matrix[row2][(col2 - 1 + SIZE) % SIZE]);
            }
            else if (col1 == col2) {
                decrypted += static_cast<char>(matrix[(row1 - 1 + SIZE) % SIZE][col1]);
                decrypted += static_cast<char>(matrix[(row2 - 1 + SIZE) % SIZE][col2]);
            }
            else {
                decrypted += static_cast<char>(matrix[row1][col2]);
                decrypted += static_cast<char>(matrix[row2][col1]);
            }
        }

        return restoreText(decrypted);
    }

    void printMatrixPartial() {
        cout << "Матрица Плейфера (первые 4 строки):\n";
        for (int i = 0; i < 4 && i < SIZE; i++) {
            cout << "Строка " << i << ": ";
            for (int j = 0; j < 4 && j < SIZE; j++) {
                unsigned char c = matrix[i][j];
                if (c >= 32 && c < 127) {
                    cout << c << ' ';
                }
                else {
                    printf("%02X ", c);
                }
            }
            cout << "...\n";
        }
    }
};

// Вспомогательные функции для бинарного чтения/записи
string readBinaryFile(const string& filename) {
    ifstream file(filename, ios::binary | ios::ate);
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл для чтения: " + filename);
    }

    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    string result(size, '\0');
    if (!file.read(&result[0], size)) {
        throw runtime_error("Ошибка чтения файла: " + filename);
    }

    return result;
}

bool writeBinaryFile(const string& filename, const string& content) {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Ошибка: Не удалось создать файл '" << filename << "'\n";
        return false;
    }

    if (!file.write(content.data(), content.size())) {
        cerr << "Ошибка записи в файл '" << filename << "'\n";
        return false;
    }

    return true;
}

bool validateTextPlayfair(const string& text, const string& language) {
    return true;
}

string encryptPlayfairCipher(const string& text, const string& key) {
    PlayfairCipherCore cipher(key);
    return cipher.encrypt(text);
}

string decryptPlayfairCipher(const string& text, const string& key) {
    PlayfairCipherCore cipher(key);
    return cipher.decrypt(text);
}

void runPlayfairCipher() {
    string text, inputFilename, outputFilename, key;
    int action;
    char useFile;

    cout << "ШИФР ПЛЕЙФЕРА (ASCII 256 символов)\n";
    cout << "Использует все символы ASCII (0-255)\n";
    cout << "Заполнитель для одинаковых символов: символ 255 (0xFF)\n";
    cout << "Примечание: для работы с файлами используется бинарный режим\n";

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
                text = readBinaryFile(inputFilename);
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

    cout << "Введите ключ (может содержать любые символы): ";
    getline(cin, key);

    if (key.empty()) {
        cout << "Ошибка: ключ не может быть пустым.\n";
        return;
    }

    cout << "Выберите действие: 1 - Шифрование, 2 - Дешифрование: ";
    cin >> action;

    try {
        PlayfairCipherCore cipher(key);
        string result;

        if (action == 1) {
            result = cipher.encrypt(text);
            cout << "Зашифрованный текст: " << result << endl;
        }
        else {
            result = cipher.decrypt(text);
            cout << "Расшифрованный текст: " << result << endl;
        }

        char showMatrix;
        cout << "Показать часть матрицы Плейфера? (y/n): ";
        cin >> showMatrix;
        if (showMatrix == 'y' || showMatrix == 'Y') {
            cipher.printMatrixPartial();
        }

        cout << "Сохранить в файл? (y/n): ";
        cin >> useFile;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (useFile == 'y' || useFile == 'Y') {
            cout << "Введите имя выходного файла: ";
            getline(cin, outputFilename);

            if (writeBinaryFile(outputFilename, result)) {
                cout << "Результат сохранен в файл: " << outputFilename << endl;
            }
        }
    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}