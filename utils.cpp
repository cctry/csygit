#include "utils.hpp"
#include <sys/stat.h>
#include "cerror.hpp"
#include "repo.hpp"

using namespace std;
int util::get_file_size(string path) {
    if (!util::is_file_exist(path)) {
        cerror::occur_error("file is not existed: " + path);
    }
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
    if (!util::is_file_exist(path)) {
        cerror::occur_error("file is not existed: " + path);
    }
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
                        string(dirp->d_name) != "." &&
                        string(dirp->d_name) == DEFAULT_REPO_NAME) {
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

int get_permission(string path, int mode) {  // 1 for file, 2 for dir
    struct stat buf;
    if (stat(path.c_str(), &buf) == -1) {
        cout << string("cannnot access file: " + path) << endl;
    }
    if (S_ISDIR(buf.st_mode) && mode == 1) {  // is a dir but want file
        cout << string("mistaken a dir as a file: " + path) << endl;
    } else if (!S_ISDIR(buf.st_mode) && mode == 2) {  // is a file but want dir
        cout << string("mistaken a file as a dir: " + path) << endl;
    } else {
        return (int)(buf.st_mode) & 511;
    }
}

int util::get_file_permission(string path) {
    return get_permission(path, 1);
}

int util::get_dir_permission(string path) {
    return get_permission(path, 2);
}
