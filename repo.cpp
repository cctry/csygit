#include "repo.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "Object.hpp"
#include "cerror.hpp"
#include "utils.hpp"

#define DEFAULT_REPO_NAME ".repo"
#define DEFAULT_OBJECTS_NAME "objects"
#define DEFAULT_OBJECTS_PATH DEFAULT_REPO_NAME "/" DEFAULT_OBJECTS_NAME "/"
using namespace cerror;
using namespace std;

int cmkdir(string path, int mode) {
    return mkdir(path.c_str(), mode);
}

void repo::init_db() {
    ostringstream buffer;
    buffer << DEFAULT_REPO_NAME;
    // create ./DEFAULT_REPO_NAME
    if (cmkdir(buffer.str(), 0700) < 0) {
        // mkdir failed
        occur_error("Failed to create repository");
    }
    // create ./DEFAULT_REPO_NAME/DEFAULT_OBJECTS_NAME
    buffer << '/' << DEFAULT_OBJECTS_NAME;
    if (cmkdir(buffer.str(), 0700) < 0) {
        // mkdir failed
        occur_error("Failed to create repository");
    }

    buffer << '/';
    char buf[3] = {0, 0, 0};
    string sbuf;
    for (int i = 0; i < 256; i++) {  // hex from 0 ro 256
        sprintf(buf, "%02x", i);
        sbuf = buf;
        string path = buffer.str() + sbuf;
        if (cmkdir(path, 0700) < 0) {
            // mkdir failed
            occur_error("Failed to create repository db");
        }
    }
}

#define SLASH (string("/"))

string get_obj_path(string hash) {
    string catalog = hash.substr(0, 2);
    string filename = hash.substr(2);
    return string(DEFAULT_OBJECTS_PATH) + catalog + SLASH +
           filename;  //.repo/objects/catalog/filename
}

void repo::save_obj(Object& obj) {
    string hash = obj.get_hash();
    string path = get_obj_path(hash);
    if (util::is_file_exist(path)) {  // if the object exist.
        // TODO hash confilct
    } else {
        ofstream f(path.c_str(), ios::out | ios::binary);
        if (f.bad()) {
            occur_error("Cannot create file");
        }
        f.close();
        obj.save_as_obj(path);
    }
}

string repo::load_obj(string hash) {
    return get_obj_path(hash);
}