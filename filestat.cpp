#include <string>
#include <iostream>
#include <filesystem>
#include <map>
#include <vector>

namespace fs = std::filesystem;

class Filestat {

public:
    Filestat(std::string path) {
        folderPath = path;
    }

    void printAllFiles() {
        if (!fs::is_directory(folderPath)) {
            std::cout << "Path is not folder" << std::endl;
            return;
        }
        std::map<std::string, uint32_t> format2num;
        for (const auto &entry: fs::directory_iterator(folderPath)) {
            if(!fs::is_directory(entry)){
                std::string path = entry.path().string();
                auto result = split(path, '.');
                std::string format = result[result.size() - 1];
                if (auto search = format2num.find(format); search != format2num.end()){
                    int number = search->second+1;
                    search->second = number;
                }
                else{
                    format2num.insert(std::pair<std::string, uint32_t>("."+format,1));
                }
            }
        }
        if(format2num.empty()){
            std::cout << "There is no files" <<std::endl;
            return;
        }
        std::cout << "File format -> files count" <<std::endl;
        for (const auto& [key, value] : format2num) {
            std::cout << key << " -> " << value << std::endl;
        }
    }

private:
    std::string folderPath;

    std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> result;
        std::stringstream ss(s);
        std::string item;

        while (getline(ss, item, delim)) {
            result.push_back(item);
        }

        return result;
    }
};