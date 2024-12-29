/*
Кузнецов Павел ПС-23, среда CLION
20. Строки текстового  файла  содержат  фамилии  студентов.
Определить   порядковый   номер   байта,  начиная  с  которого
располагается первая  по  алфавиту  фамилия.  Заменить  данную
фамилию  в исходном файле символами '*',  не переписывая этого
файла (8).
 */

#include <iostream>
#include <fstream>
#include <windows.h>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::string file_name;
    std::cin >> file_name;

    std::wifstream in_file;

    in_file.open(file_name, std::ios::in|std::ios::binary);
    in_file.seekg(0, std::ios::beg);

    if (!in_file)
    {
        std::cout << "Файл не открыт" << std::endl;
        return -1;
    }

    std::wstring first_word;
    long long first_pos;
    first_pos = 0;
    std::wstring new_word;
    long long new_pos;

    in_file >> first_word;

    while (!in_file.eof())
    {
        new_pos = in_file.tellg() + (long long)2;
        in_file >> new_word;

        if (new_word < first_word)
        {
            first_word = new_word;
            first_pos = new_pos;
        }
    }

    std::wfstream out_file;
    out_file.open(file_name, std::ios::in|std::ios::out|std::ios::binary);

    out_file.seekp(first_pos, std::ios::beg);

    wchar_t ch = '*';
    for (int i = 0; i < first_word.length(); ++i) {
        out_file << ch;
    }

    in_file.close();
    out_file.close();
    return 0;
}