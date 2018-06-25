#include "utils.hpp"
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
#include <vector>
#include "cerror.hpp"
#include "repo.hpp"

#ifndef DEFAULT_REPO_NAME
#define DEFAULT_REPO_NAME ".repo"
#endif

using namespace std;
int util::get_file_size(const string& path) {
    if (!util::is_file_exist(path)) {
        cerror::occur_error("file is not existed: " + path);
    }
    const char* filename = path.c_str();
    struct stat statbuf;
    stat(filename, &statbuf);
    int size = statbuf.st_size;
    return size;
}

string util::get_file_content(const string& path) {
    if (!util::is_file_exist(path)) {
        cerror::occur_error("file is not existed: " + path);
    }
    ifstream t(path.c_str());
    string str((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
    t.close();
    return str;
}

bool util::is_file_exist(const string& path) {
    if (access(path.c_str(), F_OK) == -1) {
        return false;
    } else {
        return true;
    }
}

bool util::is_dir_exist(const string& path) {
    if (opendir(path.c_str()) == NULL)
        return false;
    else
        return true;
}

#define FILE_MODE 8
#define DIR_MODE 4
#define ALL_MODE -1

vector<string> scan_dir(int mode, const string& dirname) {
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

vector<string> util::get_all_files(const string& path) {
    return scan_dir(FILE_MODE, path);
}

vector<string> util::get_all_dirs(const string& path) {
    return scan_dir(DIR_MODE, path);
}

int get_permission(const string& path, int mode) {  // 1 for file, 2 for dir
    struct stat buf;
    if (stat(path.c_str(), &buf) == -1) {
        cerror::occur_error(string("cannot access file: ") + path);
    }
    if (S_ISDIR(buf.st_mode) && mode == 1) {  // is a dir but want file
        cerror::occur_error(string("mistaken a dir as a file: ") + path);
    } else if (!S_ISDIR(buf.st_mode) && mode == 2) {  // is a file but want dir
        cerror::occur_error(string("mistaken a file as a dir: ") + path);
    } else {
        return (int)(buf.st_mode) & 511;
    }
}

int util::get_file_permission(const string& path) {
    return get_permission(path, 1);
}

int util::get_dir_permission(const string& path) {
    return get_permission(path, 2);
}

vector<string> util::get_lines(ifstream& f) {
    string line;
    vector<string> res;
    while (!f.eof()) {
        std::getline(f, line);
        res.push_back(line);
    }
    return res;
}

int util::load_a_int(ifstream& f) {
    int temp = 0;
    f.read((char*)(&temp), sizeof(int));
    if (temp) {
        return temp;
    } else {
        return -1;
    }
}
