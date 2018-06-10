#include "Blob.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include "cerror.hpp"
#include "repo.hpp"
#include "sha1.hpp"
#include "utils.hpp"

using namespace std;
using namespace cerror;
// static construct function for blob from a normal file
Blob* Blob::get_blob_from_file(string path) {
    Blob* res = new Blob;
    res->init_hdr();
    sha1 s("");
    s.get_sha1_from_file(path.c_str());
    res->path = path;
    res->hash = string(s.hex);
    res->content = util::get_file_content(path);
    int size = util::get_file_size(path);
    res->hdr.size = size;
    res->hdr.path_len = path.length();
    return res;
}

// static construct function for blob from a obj file
Blob* Blob::get_blob_from_db(string hash) {
    // open the object in db
    Blob* res = new Blob;
    res->hash = hash;
    string obj_path = repo::load_obj(hash);
    ifstream f(obj_path.c_str(), ios::binary);

    // filled with hdr information
    obj_hdr_t& hdr = res->get_hdr();
    f.read((char*)(&(res->hdr)), sizeof(obj_hdr_t));
#ifdef DEBUG
    cout << hdr.size << endl;
#endif
    // check the header of obj file
    if (hdr.signature != SIGNATURE) {
        occur_error("courrpted file.\n");  // TODO
    }

    // get the orignal path of file
    char* path = new char[hdr.path_len + 1];
    f.read(path, hdr.path_len * sizeof(char));
    path[hdr.path_len] = '\0';
    res->path = string(path);
    delete[] path;

    // get content
    stringstream stemp;
    for (int i = 0; i < hdr.size; i++) {
        stemp << (char)f.get();
    }
    f.close();
    res->content = stemp.str();
    return res;
}

string& Blob::get_content() {
    return content;
}

/*
 * The structure of blob file.
 * |version|signature|size|path_len|type|time|path|
 * |content|
 */
void Blob::save_as_obj(string obj_path) {
    ofstream f(obj_path.c_str(), ios::binary);
    f.write((char*)(&hdr), sizeof(obj_hdr_t));
    f.write(path.c_str(), hdr.path_len);
    f.write(content.c_str(), content.length());
    f.close();
}

// save blob to its original path
void Blob::save_as_file() {
    ofstream f(path.c_str(), ios::trunc);
    f.write(content.c_str(), content.length());
    f.close();
}

#ifdef BLOB_TEST

Blob* Blob::load_obj(string obj_path) {
    Blob* res = new Blob;
    ifstream f(obj_path.c_str(), ios::binary);
    // filled with hdr information
    obj_hdr_t& hdr = res->get_hdr();
    f.read((char*)(&(res->hdr)), sizeof(obj_hdr_t));
    // check the header of obj file
    if (hdr.signature != SIGNATURE) {
        occur_error("courrpted file.\n");  // TODO
    }
    // get the orignal path of file
    char* path = new char[hdr.path_len + 1];
    f.read(path, hdr.path_len * sizeof(char));
    path[hdr.path_len] = '\0';
    res->path = string(path);
    delete[] path;
    // get content
    stringstream stemp;
    for (int i = 0; i < hdr.size; i++) {
        stemp << (char)f.get();
    }
    f.close();
    res->content = stemp.str();
    return res;
}

int main(int args, char** argv) {
    Blob* blob = Blob::get_blob_from_file(string(argv[1]));
    cout << blob->get_hash() << endl;
    string src = string(argv[1]) + string(".tmp");
    // save obj file
    blob->save_as_obj(src);
    delete blob;
    // read obj file
    blob = Blob::load_obj(src);
    cout << blob->get_content() << endl;
    delete blob;
    return 0;
}

#endif