#include <arpa/inet.h>
#include <unistd.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

const int BUFFER_SIZE = 1024;

int executeCommand(const char* command, char* outputBuffer,
                   size_t outputBufferSize) {
    auto start = std::chrono::steady_clock::now();
    FILE* pipe = popen(command, "r");
    if (!pipe) {
        return -1;
    }

    std::string result;
    char buffer[128];

    while (!feof(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
        // делаем timeout
        if (std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() - start)
                .count() > 100) {
            result = "Timeout exceed!";
            strncpy(outputBuffer, result.c_str(), outputBufferSize - 1);
            outputBuffer[outputBufferSize - 1] = '\0';
            return -1;
        }
    }

    strncpy(outputBuffer, result.c_str(), outputBufferSize - 1);
    outputBuffer[outputBufferSize - 1] = '\0';

    int status = pclose(pipe);

    return status;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "You should enter port number as argument!" << '\n';
        return -1;
    }

    int serverSocket;
    sockaddr_in serverAddress, clientAddress;

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Error creating socket" << '\n';
        return -1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(std::atoi(argv[1]));
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress,
             sizeof(serverAddress)) < 0) {
        std::cerr << "Error binding socket" << '\n';
        close(serverSocket);
        return -1;
    }

    if (listen(serverSocket, 10) < 0) {
        std::cerr << "Error listening for connections" << '\n';
        close(serverSocket);
        return -1;
    }

    std::cout << "Server is listening on port " << std::atoi(argv[1]) << '\n';

    socklen_t clientLength = sizeof(clientAddress);

    while (true) {
        int clientSocket = accept(
            serverSocket, (struct sockaddr*)&clientAddress, &clientLength);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection." << '\n';
            continue;
        }

        char buffer[BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer));

        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            std::cerr << "Error receiving command." << '\n';
            close(clientSocket);
            continue;
        }
        std::cout << "Received: " << buffer << '\n';

        // Выполняем команду
        char outputBuffer[BUFFER_SIZE];
        memset(outputBuffer, 0, sizeof(outputBuffer));

        int exitCode =
            executeCommand(buffer, outputBuffer, sizeof(outputBuffer));

        // Отправляем результаты на клиент
        std::string response =
            "--------\nExit code: " + std::to_string(exitCode) +
            "\nBuffer:" + outputBuffer + "\n--------\n";
        send(clientSocket, response.c_str(), response.length(), 0);

        close(clientSocket);
    }

    close(serverSocket);

    return 0;
}
