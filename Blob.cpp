#include "Blob.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Object.hpp"
#include "cerror.hpp"
#include "repo.hpp"
#include "sha1.hpp"
#include "utils.hpp"

using namespace cerror;

// static construction function for blob from a normal file
Blob* Blob::get_blob_from_file(const std::string& path) {
    using namespace std;
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
    res->size = util::get_file_size(path);
    res->hdr.path_len = path.length();
    return res;
}

// static construct function for blob from a obj file
Blob* Blob::get_blob_from_db(const std::string& hash) {
    // open the object in db
    std::string obj_path = repo::load_obj(hash);
    return load_obj(obj_path, hash);
}

Blob* Blob::load_obj(const std::string& obj_path, const std::string& hash) {
    Blob* res = new Blob;
    res->hash = hash;
    std::ifstream f(obj_path.c_str(), std::ios::binary);

    // filled with hdr information
    if (res->load_hdr(f) == -1) {
        occur_error("corrupted file: " + obj_path);  // TODO
    }

    // get permission
    res->permission = util::load_a_int(f);

    // get size
    res->size = util::load_a_int(f);

    // get path
    res->path = util::load_string_from_file(res->hdr.path_len, f);

    // get content
    std::stringstream stemp;
    for (int i = 0; i < res->size; i++) {
        stemp << (char)f.get();
    }
    f.close();
    res->content = stemp.str();
    return res;
}

/*
 * The structure of blob file.
 * |version|signature|path_len|time|  <--- header
 * |permission|size|
 * |path|
 * |content|
 */
void Blob::save_as_obj(const std::string& obj_path) {
    std::ofstream f(obj_path.c_str(), std::ios::binary);
    f.write((char*)(&hdr), sizeof(obj_hdr_t));
    f.write((char*)(&permission), sizeof(int));
    f.write((char*)(&size), sizeof(int));
    f.write(path.c_str(), hdr.path_len);
    f.write(content.c_str(), content.length());
    f.close();
}

// save blob to its original path
void Blob::save_as_file() {
    std::ofstream f(path.c_str(), std::ios::trunc);
    f.write(content.c_str(), content.length());
    f.close();
}

Blob::Blob(Blob&& blob) {
    path = std::move(blob.path);
    content = std::move(blob.content);
    permission = blob.permission;
    size = blob.size;
    hdr = std::move(blob.hdr);
}

#ifdef BLOB_TEST
int main(int args, char** argv) {
    Blob* blob = Blob::get_blob_from_file(string(argv[1]));
    cout << blob->get_hash() << endl;
    string dest = string(argv[1]) + string(".tmp");
    obj_hdr_t& hdr = blob->get_hdr();
    // save obj file
    blob->save_as_obj(dest);
    delete blob;
    // read obj file
    blob = Blob::load_obj(dest, "123");
    hdr = blob->get_hdr();
    cout << "permission: " << blob->get_permission() << endl;
    cout << "size: " << blob->get_size() << endl;
    cout << blob->get_path() << endl;
    cout << blob->get_content() << endl;
    delete blob;
    return 0;
}
#endif
