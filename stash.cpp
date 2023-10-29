#include <iostream>
#include <fstream>
#include <vector>

std::vector<char> readFile(const std::string &fileName) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Can not open a file");
    }
    std::vector<char> fileBytes;
    char byte;
    while (file.get(byte)) {
        fileBytes.push_back(byte);
    }
    file.close();
    return fileBytes;
}

void saveFile(const std::string &fileName, const std::vector<char> &fileBytes) {
    std::ofstream output(fileName, std::ios::binary);
    if (!output.is_open()) {
        throw std::runtime_error("Can not open a file");
    }
    for (const char &byte: fileBytes) {
        output.put(byte);
    }
    output.flush();
    output.close();
}

void distort(const std::string &fileName) {
    std::vector<char> fileBytes;
    fileBytes.push_back((char) 0x12345);
    try {
        auto file = readFile(fileName);
        fileBytes.insert(fileBytes.end(), file.begin(), file.end());
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << '\n';
        return;
    }

    // Проверяем, не исказили ли два раза
    if (fileBytes[0] == (char) 0x12345 && fileBytes[1] == (char) 0x12346) {
        std::cerr << "You can't distort already distorted file!" << '\n';
        return;
    }
    for (char &byte: fileBytes) {
        byte += 1;
    }
    try {
        saveFile(fileName, fileBytes);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << '\n';
    }
}

void recovery(const std::string &fileName) {
    std::vector<char> fileBytes;
    try {
        fileBytes = readFile(fileName);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << '\n';
        return;
    }

    // Проверяем, есть ли байт, которйы поставили по время искажения
    if (fileBytes[0] != (char) 0x12346) {
        std::cerr << "You can't restore before distorting!" << '\n';
        return;
    }
    std::vector<char>::const_iterator first = fileBytes.begin() + 1;
    std::vector<char>::const_iterator last = fileBytes.end();
    std::vector<char> data(first, last);
    for (char &byte: data) {
        byte -= 1;
    }
    try {
        saveFile(fileName, data);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << '\n';
    }
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Wrong count of arguments" << '\n';
        return 1;
    }

    const std::string fileName = argv[1];
    const std::string mode = argv[2];

    if (mode == "distort") {
        distort(fileName);
    } else if (mode == "restore") {
        recovery(fileName);
    } else {
        std::cerr << "Wrong argument value, user distort/restore" << '\n';
        return 1;
    }

    return 0;
}