#include "util.hpp"
#include <sys/stat.h>
using namespace std;
int get_file_size(string path) {
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
string get_file_content(string path) {
    ifstream t(path.c_str());
    string str((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
    return str;
}
