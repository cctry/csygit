#include "utils.hpp"
#include <sys/stat.h>
using namespace std;
int util::get_file_size(string path) {
    const char* filename = path.c_str();
    struct stat statbuf;
    stat(filename, &statbuf);
    int size = statbuf.st_size;
    return size;
}

#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
string util::get_file_content(string path) {
    ifstream t(path.c_str());
    string str((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
    t.close();
    return str;
}

#include <unistd.h>

bool util::is_file_exist(string path) {
    if (access(path.c_str(), F_OK) == -1) {
        return false;
    } else {
        return true;
    }
}