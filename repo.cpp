#include "repo.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string>
#include "Object.hpp"
#include "cerror.hpp"
#include "utils.hpp"

using namespace cerror;
using namespace std;

void repo::init_db() {
    string buffer = "";
    buffer += DEFAULT_REPO_NAME;
    // create ./DEFAULT_REPO_NAME
    if (util::cmkdir(buffer, 0700) < 0) {
        // mkdir failed
        occur_error("Failed to create repository");
    }

    // create ./DEFAULT_REPO_NAME/HEAD file
    ofstream HEAD_file(util::cat_string(buffer, SLASH, HEAD_FILE).c_str(),
                       ios::trunc);
    HEAD_file << "heads/master";
    HEAD_file.close();

    // create ./DEFAULT_REPO_NAME/heads
    if (util::cmkdir(util::cat_string(buffer, SLASH, DEFAULT_HEADS_PATH),
                     0700) < 0) {
        // mkdir failed
        occur_error("Failed to create repository");
    }

    // create ./DEFAULT_REPO_NAME/DEFAULT_OBJECTS_NAME
    buffer += SLASH;
    buffer += DEFAULT_OBJECTS_NAME;
    if (util::cmkdir(buffer, 0700) < 0) {
        // mkdir failed
        occur_error("Failed to create repository");
    }

    buffer += SLASH;
    char buf[3] = {0, 0, 0};
    string sbuf;
    for (int i = 0; i < 256; i++) {  // hex from 0 ro 256
        sprintf(buf, "%02x", i);
        sbuf = buf;
        string path = buffer + sbuf;
        if (util::cmkdir(path, 0700) < 0) {
            // mkdir failed
            occur_error("Failed to create repository db");
        }
    }
}

// rebuild the actual path of a obj in the obj db
string get_obj_path(const string& hash) {
    string catalog = hash.substr(0, 2);
    string filename = hash.substr(2);
    return util::cat_string(DEFAULT_OBJECTS_PATH, catalog, SLASH,
                            filename);  //.repo/objects/catalog/filename
}

string repo::load_obj(const string& hash) {
    return get_obj_path(hash);
}

// save a obj
void repo::save_obj(Object& obj) {
    string hash = obj.get_hash();
    string path = get_obj_path(hash);
    if (util::is_file_exist(path)) {  // if the object exist.
        return;                       // TODO hash confilct
    } else {
        ofstream f(path.c_str(), ios::out | ios::binary);
        if (f.bad()) {
            occur_error("Cannot create file");
        }
        f.close();
        obj.save_as_obj(path);
    }
}

/* Return the current head hash
 * If this is the very first commit and there is no head in heads,
 * return an empty std::string
 */
const string repo::get_head_hash() {
    string head_path = util::get_file_content(HEAD_PATH);
    string path = util::cat_string(util::get_cwd(), SLASH, DEFAULT_REPO_NAME,
                                   SLASH, head_path);
    if (!util::is_file_exist(path)) {  // first commit
        string str;
        return str;
    }
    string str = util::get_file_content(path);
    return str;
}

ofstream repo::create_head() {
    string head_path = util::get_file_content(HEAD_PATH);
    string path = util::cat_string(util::get_cwd(), SLASH, DEFAULT_REPO_NAME,
                                   SLASH, head_path);
    ofstream t(path.c_str(), ios::trunc);
    t.close();
    ofstream f(path.c_str());
    return f;
}
