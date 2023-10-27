
#include <string>
#include "hide.h"
#include <iostream>
#include <sys/stat.h>
#include <filesystem>
#include <map>
#include <vector>
#include <fstream>
#include <unistd.h>

namespace fs = std::filesystem;

class Hide {
public:
    Hide(std::string fileName) {
        this->fileName = fileName;
    }

    void hideFile() {
        createHiddenFolder();
    }

private:
    std::string folderPath = "C:\\Users\\a.petropavlovskiy\\Desktop\\test-folder\\hiden-folder";
    std::string fileName;

    std::string createHiddenFolder() {
        //const char* dir = "C:/Users/apples";

        // Structure which would store the metadata
        struct stat sb;
        if (stat(folderPath.c_str(), &sb) == 0) {
            // Folder is already exist
            std::cout << "The path is valid!" << std::endl;
        } else {
            fs::create_directory(folderPath);
//            fs::permissions(folderPath, fs::perms::group_write);
//            fs::permissions(folderPath, fs::perms::mask);
            chmod(folderPath.c_str(), 003);
            //fs::permissions(folderPath, fs::perms::group_read);
            std::cout << "Folder has been created" << std::endl;
            try {
                auto resultFile = folderPath + "\\" + fileName;
                auto ptr = fopen(resultFile.c_str(),"w");
                fprintf(ptr,"%d",123);
                fclose(ptr);
//                std::ofstream myfile;
//                myfile.open(resultFile);
//                myfile << "Writing this to a file.\n";
//                myfile.close();
//                auto qw = myfile.exceptions();
            } catch (const char *error_message) {
                std::cout << error_message << std::endl;
            }
            std::cout << "File has been created" << std::endl;
        }

    }

};