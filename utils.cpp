#include "utils.hpp"
#include <sys/stat.h>
#include "cerror.hpp"
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

#include <dirent.h>
#define FILE_MODE 8
#define DIR_MODE 4
#define ALL_MODE -1

vector<string> scan_dir(int mode, string dirname) {
    DIR* dp;
    struct dirent* dirp;
    if ((dp = opendir(dirname.c_str())) == NULL)
        cerror::occur_error("Cannot open " + dirname);
        //cout << "Cannot open " << dirname << endl;
    vector<string> res;
    while ((dirp = readdir(dp)) != NULL) {
        switch (mode) {
            case FILE_MODE:
                if (dirp->d_type == 8)
                    res.push_back(string(dirp->d_name));
                break;
            case DIR_MODE:
                if (dirp->d_type == 4) {
                    if (string(dirp->d_name) != ".." &&
                        string(dirp->d_name) != ".") {
                        res.push_back(string(dirp->d_name));
                    }
                }
                break;
            default:
                res.push_back(string(dirp->d_name));
        }
    }
    return res;
}

vector<string> util::get_all_files(string path) {
    return scan_dir(FILE_MODE, path);
}

vector<string> util::get_all_dirs(string path) {
    return scan_dir(DIR_MODE, path);
}
