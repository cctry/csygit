#include "Commit.hpp"
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include "Blob.hpp"
#include "Dir.hpp"
#include "repo.hpp"
#include "sha1.hpp"
#include "utils.hpp"

Commit::~Commit() {
    delete dir;
}

Commit* Commit::get_from_dir(const std::string& path) {
    Commit* res = new Commit;
    res->init_hdr();
    res->dir = Dir::get_dir_from_path(path);
    res->next_hash = util::zero_hash;
    // The hash of Commit object is hash(path + dir.hash)
    res->hash =
        sha1::hash_a_string(util::cat_string(path, res->dir->get_hash()));

    std::string head_hash = repo::get_head_hash();
    if (!head_hash.empty()) {
        res->pre_hash = std::move(head_hash);
    }
    std::ofstream f = repo::create_head();
    f << res->hash;
    f.close();
    return res;
}

Commit* Commit::get_from_db(const std::string& hash) {
    Commit* res = new Commit;
    res->hash = hash;
    std::string obj_path = repo::load_obj(hash);
    std::ifstream f(obj_path, std::ios::binary);
    res->load_hdr(f);
    res->pre_hash = util::load_string_from_file(HASH_LEN, f);
    auto items = util::get_lines(f);
    auto iter = items.begin();
    res->pre_hash = *(iter++);
    res->next_hash = *(iter++);
    std::string dir_hash = *(iter++);
    for (; iter != items.end(); iter++) {
        res->content += *iter;
    }
    res->dir = Dir::get_dir_from_db(dir_hash);
    return res;
}

/*
 * The structure of dir object file:
 * |version|signature|path_len|time|   <---- header
 * |pre_hash|\n
 * |next_hash|\n
 * |dir_hash|\n
 * |content|
 */
void Commit::save_as_obj(const std::string& obj_path) {
    std::ofstream f(obj_path.c_str(), std::ios::binary);
    f.write((char*)(&hdr), sizeof(obj_hdr_t));
    f << pre_hash << "\n"
      << next_hash << "\n"
      << dir->get_hash() << "\n"
      << content << "\n";
    repo::save_obj(*dir);
    f.close();
}

void Commit::print() {
    std::cout << hash << std::endl;
    std::cout << "Date: " << std::ctime(&(hdr.time)) << std::endl;
    std::cout << content << std::endl << std::endl;
}
