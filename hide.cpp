#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "You should pass filename as argument" << '\n';
        return 1;
    }

    const std::string hiddenDirectory = "hidden_folder";
    const std::string file2Hide = argv[1];

    if (!fs::exists(file2Hide)) {
        std::cerr << "File does not exist" << '\n';
        return 1;
    }

    if (!fs::is_directory(hiddenDirectory)) {
        if (!fs::create_directory(hiddenDirectory)) {
            std::cerr << "Error while creating hidden dir" << '\n';
            return 1;
        }
        fs::permissions(hiddenDirectory, fs::perms::owner_write | fs::perms::owner_exec);
    }
    try {
        fs::rename(file2Hide, fs::path(hiddenDirectory) / fs::path(file2Hide).filename());
        std::cout << "File '" << file2Hide << "' hidden" << '\n';
    } catch (const fs::filesystem_error &e) {
        std::cerr << "Error while hiding a file: " << e.what() << '\n';
        return 1;
    }
    return 0;
}