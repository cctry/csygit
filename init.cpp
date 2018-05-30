#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include "cerror.hpp"

#define DEFAULT_REPO_PATH ".repo"
#define OBJECT_PATH "objects"

using namespace std;

void init_repo() {
    ostringstream buffer;
    buffer << DEFAULT_REPO_PATH;
    // create ./DEFAULT_REPO_PATH
    if (mkdir(buffer.str().c_str(), 0700) < 0) {
        // mkdir failed
        occur_error("Failed to create repository");
    }
    // create ./DEFAULT_REPO_PATH/OBJECT_PATH
    buffer << '/' << OBJECT_PATH;
    if (mkdir(buffer.str().c_str(), 0700) < 0) {
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
        if (mkdir(path.c_str(), 0700) < 0) {
            // mkdir failed
            occur_error("Failed to create repository db");
        }
    }
}