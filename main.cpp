#include <iostream>
#include <string>
#include <limits>  
#include "AffineCipher.h"
#include "PlayfairCipher.h"
#include "gronsfeld.h"
#include "common.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
using namespace std;

extern string programLanguage;

bool authenticate() {
    const string staticPassword = "qwerty123321";
    string inputPassword;
    int attempts = 3;

    while (attempts > 0) {
        cout << "Введите пароль (" << attempts << " попыток осталось): ";
        getline(cin, inputPassword);

        if (inputPassword == staticPassword) {
            return true;
        }
        else {
            cout << "Неверный пароль!\n";
            attempts--;
        }
    }

    cout << "Доступ запрещен! Слишком много неудачных попыток.\n";
    return false;
}

void displayFileContent() {
    string filename;

    cout << "Введите имя файла для отображения: ";
    getline(cin, filename);

    try {
        string content = readFile(filename);
        cout << "=== Содержимое файла '" << filename << "' ===\n";
        cout << content << "\n";
        cout << "=== Конец файла ===\n";
    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << "\n";
    }
}

void fileOperationsMenu() {
    int choice;

    do {
        cout << "\n=== Операции с файлами ===\n";
        cout << "1. Создать новый файл и записать текст\n";
        cout << "2. Прочитать содержимое файла\n";
        cout << "3. Показать содержимое файла в консоли\n";
        cout << "4. Вернуться в главное меню\n";
        cout << "Введите ваш выбор (1-4): ";

        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1: {
            string filename, content;
            cout << "Введите имя файла: ";
            getline(cin, filename);
            cout << "Введите содержимое: ";
            getline(cin, content);
            createFile(filename, content);
            break;
        }
        case 2: {
            string filename;
            cout << "Введите имя файла для чтения: ";
            getline(cin, filename);
            try {
                string content = readFile(filename);
                cout << "Файл успешно прочитан. Длина содержимого: " << content.length() << " символов.\n";
            }
            catch (const exception& e) {
                cerr << "Ошибка: " << e.what() << "\n";
            }
            break;
        }
        case 3:
            displayFileContent();
            break;
        case 4:
            cout << "Возврат в главное меню...\n";
            break;
        default:
            cout << "Неверный выбор!\n";
            break;
        }
    } while (choice != 4);
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    if (!authenticate()) {
        return 1;
    }

    int choice;

    do {
        cout << "\n=== Программа Криптографии ===\n";
        cout << "1. Аффинный шифр\n";
        cout << "2. Шифр Плейфера\n";
        cout << "3. Шифр Гронсфельда\n";
        cout << "4. Операции с файлами\n";
        cout << "5. Выход\n";
        cout << "Введите ваш выбор (1-5): ";

        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            runAffineCipher();
            break;
        case 2:
            runPlayfairCipher();
            break;
        case 3:
            runGronsfeldCipher();
            break;
        case 4:
            fileOperationsMenu();
            break;
        case 5:
            cout << "Выход...\n";
            break;
        default:
            cout << "Неверный выбор!\n";
            break;
        }
    } while (choice != 5);

    return 0;
}
