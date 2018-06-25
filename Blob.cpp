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

// static construction function for blob from a normal file
Blob* Blob::get_blob_from_file(const string& path) {
    Blob* res = new Blob;
    res->init_hdr();
#ifdef DEBUG
    cout << "new a blob: " << path << endl;
#endif
    sha1 s("");
    s.get_sha1_from_file(path.c_str());
    res->path = path;
    res->hash = string(s.hex);
    res->permission = util::get_file_permission(path);
#ifdef DEBUG
    cout << "hash: " << res->hash << endl;
#endif
    res->content = util::get_file_content(path);
    int size = util::get_file_size(path);
    res->hdr.size = size;
    res->hdr.path_len = path.length();
    return res;
}

// static construct function for blob from a obj file
Blob* Blob::get_blob_from_db(const string& hash) {
    // open the object in db
    Blob* res = new Blob;
    res->hash = hash;
    string obj_path = repo::load_obj(hash);
    ifstream f(obj_path.c_str(), ios::binary);

    // filled with hdr information
    if (res->load_hdr(f) == -1) {
        occur_error("corrupted file: " + obj_path);  // TODO
    }

    //get permission
    int permission = util::load_a_int(f);
    if (permission == -1) {
        cerror::occur_error("corrupted file: " + obj_path);
    }
    res->permission = permission;

    // get the original path of file
    obj_hdr_t& hdr = res->get_hdr();
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

/*
 * The structure of blob file.
 * |version|signature|size|path_len|time|  <--- header
 * |permission|
 * |path|
 * |content|
 */
void Blob::save_as_obj(const string& obj_path) {
    ofstream f(obj_path.c_str(), ios::binary);
    f.write((char*)(&hdr), sizeof(obj_hdr_t));
    f.write((char*)(&permission), sizeof(int));
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
void prt_hdr(obj_hdr_t& hdr) {
    cout << "size: " << hdr.size << endl;
    cout << "path_len: " << hdr.path_len << endl;
}

Blob* Blob::load_obj(const string& obj_path) {
    Blob* res = new Blob;
    ifstream f(obj_path.c_str(), ios::binary);
    // filled with hdr information
    if (res->load_hdr(f) == -1) {
        occur_error("corrupted file: " + obj_path);  // TODO
    }

    //get permission
    int ptemp = 0;
    f.read((char*)(&ptemp), sizeof(int));
    res->permission = ptemp;
    
    // get the orignal path of file
    obj_hdr_t& hdr = res->get_hdr();
    char* path = new char[hdr.path_len + 1];
    f.read(path, hdr.path_len * sizeof(char));

    string spath = obj_path + ".path";
    ofstream f1(spath.c_str(), ios::binary);
    f1.write(path, hdr.path_len * sizeof(char));
    f1.close();

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
    obj_hdr_t& hdr = blob->get_hdr();
    cout<<"time from create "<<hdr.time<<endl;
    // save obj file
    blob->save_as_obj(src);
    delete blob;
    // read obj file
    blob = Blob::load_obj(src);
    hdr = blob->get_hdr();
    cout<<"time from load "<<hdr.time<<endl;
    prt_hdr(hdr);
    cout << "permission: " << blob->get_permission() << endl;
    cout << blob->get_path() << endl;
    cout << blob->get_content() << endl;
    
    delete blob;
    return 0;
}
#endif
