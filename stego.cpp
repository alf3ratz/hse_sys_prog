

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


void addMessageToFile(const std::string &fileName, const std::string &message) {
    std::vector<char> fileBytes;
    try {
        fileBytes = readFile(fileName);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << '\n';
        return;
    }

    int messageLength = message.size();

    for (char c: message) {
        fileBytes.push_back(c);
    }
    // Добавляем длину сообщения в конец файла
    fileBytes.insert(fileBytes.end(), (char) messageLength);
    try {
        saveFile(fileName, fileBytes);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << '\n';
    }

    std::cout << "Message has been written into file" << '\n';
}

std::string readMessageFromFile(const std::string &fileName) {
    std::vector<char> fileBytes;
    try {
        fileBytes = readFile(fileName);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << '\n';
        return "";
    }
    // Считываем длину сообщения из конца файла
    int messageLength = (int) fileBytes[fileBytes.size() - 1];
    if (messageLength < 0) {
        std::cerr << "Error occurs" << '\n';
        return "";
    }
    int index = fileBytes.size() - messageLength - 1;
    std::vector<char>::const_iterator first = fileBytes.begin() + index;
    std::vector<char>::const_iterator last = fileBytes.end() - 1;
    std::vector<char> data(first, last);

    // Считываем сообщение из файла
    std::string message;
    for (int i = 0; i < messageLength; ++i) {
        message += data[i];
    }

    return message;
}

void deleteMessageFromFile(const std::string &fileName) {
    std::vector<char> fileBytes;
    try {
        fileBytes = readFile(fileName);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << '\n';
        return;
    }
    // Считываем длину сообщения из конца файла
    int messageLength = (int) fileBytes[fileBytes.size() - 1];
    if (messageLength < 0) {
        std::cerr << "Error occurs" << '\n';
        return;
    }
    int index = fileBytes.size() - messageLength;
    std::vector<char>::const_iterator first = fileBytes.begin();
    std::vector<char>::const_iterator last = fileBytes.begin() + index;
    std::vector<char> data(first, last);

    try {
        saveFile(fileName, data);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << '\n';
    }
    std::cout << "Message has been deleted from file" << '\n';
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Wrong count of arguments" << '\n';
        return 1;
    }

    std::string mode = argv[1];
    std::string filename = argv[2];
    std::string message = argv[3];

    if (mode == "add") {
        addMessageToFile(filename, message);
    } else if (mode == "read") {
        std::string extractedMessage = readMessageFromFile(filename);
        if (!extractedMessage.empty()) {
            std::cout << "Message: " << extractedMessage << '\n';
        } else {
            std::cerr << "Message not found" << '\n';
        }
    } else if (mode == "delete") {
        deleteMessageFromFile(filename);
    } else {
        std::cerr << "Wrong argument value, use add/read/delete" << '\n';
        return 1;
    }

    return 0;
}