#include <iostream>
#include <filesystem>
#include <map>
#include <unistd.h>
#include <limits.h>


namespace fs = std::filesystem;


int main(int argc, char *argv[]) {
    char cwd[PATH_MAX];
    char *dir_path;
    if (argv[1] != nullptr) {
        dir_path = argv[1];
    } else if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        printf("Current working dir: %s\n", cwd);
        dir_path = getcwd(cwd, sizeof(cwd));
    } else {
        std::cerr << "getcwd() error" << '\n';
        return 1;
    }
    std::map<int, std::string> value2Type{
            std::make_pair(0, "none"),
            std::make_pair(-1, "not_found"),
            std::make_pair(1, "regular"),
            std::make_pair(2, "directory"),
            std::make_pair(3, "symlink"),
            std::make_pair(4, "block"),
            std::make_pair(5, "character"),
            std::make_pair(6, "fifo"),
            std::make_pair(7, "socket"),
            std::make_pair(8, "unknown"),
    };
    setlocale(LC_ALL, "rus");
    std::string current_directory = dir_path;
    std::map<std::filesystem::file_type, int> format2num;

    for (const auto &entry: fs::directory_iterator(current_directory)) {
        if (auto search = format2num.find(entry.status().type()); search != format2num.end()) {
            int number = search->second + 1;
            search->second = number;
        } else {
            format2num.insert(std::pair<std::filesystem::file_type, uint32_t>(entry.status().type(), 1));
        }
    }

    std::cout << "List of files in directory:" << '\n';
    for (const auto &pair: format2num) {
        std::cout << value2Type[(int) pair.first] << ": " << pair.second << " file" << '\n';
    }

    return 0;
}