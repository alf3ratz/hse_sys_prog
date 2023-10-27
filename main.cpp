#include <iostream>
#include "filestat.cpp"
#include "hide.cpp"


int main() {
    setlocale(LC_ALL, "rus");
    //Filestat filestat("C:\\Users\\a.petropavlovskiy\\Desktop\\blabla\\aopalliance\\aopalliance\\1.0");
    //filestat.printAllFiles();
    Hide hide("tst.txt");
    hide.hideFile();
    return 0;
}
