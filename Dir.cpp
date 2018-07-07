#include "Dir.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include "Blob.hpp"
#include "cerror.hpp"
#include "repo.hpp"
#include "sha1.hpp"
#include "utils.hpp"
#include <cassert>

/*
 * The structure of dir object file:
 * |version|signature|size|path_len|time|         <--- hdr
 * |path|permission|dir_num|blob_num|             <--- metadata
 * |hash|dir_path|\n|                             <--- content
 * |hash|blob_path|\n|
 * first is dir and followed blob
 */
void Dir::save_as_obj(const std::string& obj_path) {
    using namespace std;
    ofstream f(obj_path.c_str(), ios::binary|ios::trunc);
    f.write((char *)(&hdr), sizeof(obj_hdr_t));   // write header
    f.write(path.c_str(), hdr.path_len);          // write path
    f.write((char *)(&permission), sizeof(int));  // write permission
    int nums[2] = {(int)dirs.size(), (int)files.size()};
    f.write((char *)nums, sizeof(nums));
    
    // write content;
    for (auto dir_it = dirs.begin(); dir_it != dirs.end(); dir_it++) {
        Dir *temp = *dir_it;
        f << temp->get_hash() << temp->get_path() << endl;
    }
    for (auto blob_it = files.begin(); blob_it != files.end(); blob_it++) {
        Blob *temp = *blob_it;
        f << temp->get_hash() << temp->get_path() << endl;
    }
    f.close();
    for (Dir* dir : dirs) {
        repo::save_obj(*dir);
    }
    for (Blob* blob:files) {
        repo::save_obj(*blob);
    }
}

// load a dir from a object in db
Dir* Dir::get_dir_from_obj(const std::string& hash) {
    using namespace std;
    Dir *res = new Dir;
    res->hash = hash;
    string obj_path = repo::load_obj(hash);
    Dir::load_obj(obj_path, *res);
    return res;
}

void Dir::load_obj(const std::string& obj_path, Dir& dir) {
    ifstream f(obj_path.c_str(), ios::binary);

    // load header
    if (dir.load_hdr(f) == -1) {
        cerror::occur_error("corrupted file: " + obj_path);
    } 
    
    // load path
    obj_hdr_t& hdr = dir.get_hdr();
    dir.path = util::load_string_from_file(hdr.path_len, f);

    // load permission
    int permission = util::load_a_int(f);
    if (permission == -1) {
        cerror::occur_error("corrupted file: " + obj_path);
    }
    dir.permission = permission;

    // get number of blobs and dirs
    int dir_num = util::load_a_int(f);
    int blob_num = util::load_a_int(f);

    // get dirs and blobs
    vector<string> items = util::get_lines(f);
    vector<string> dir_items = util::slice_vector(items, 0, dir_num);
    vector<string> blob_items = util::slice_vector(items, dir_num);

    string hash_temp;
    string path_temp;

    // load dirs
    Dir *dir_temp;
    dir.dirs.reserve(dir_items.size());
    for (string dir_item : dir_items) {
        hash_temp = dir_item.substr(0, HASH_LEN);
        path_temp = dir_item.substr(HASH_LEN);
        dir_temp = Dir::get_dir_from_obj(hash_temp);
        // check path correctness
        if (dir_temp->get_path() != path_temp) {
            cerror::occur_error("Dir hash matching failed. Hash is " +
                                hash_temp);
        }
    #ifdef DIR_TEST
        cout << "load dir: " << dir_item << endl << path_temp << endl;
    #endif
        dir.dirs.push_back(std::move(dir_temp));
    }
    
    // load blobs
    Blob *blob_temp;
    dir.files.reserve(blob_items.size());
    for (string blob_item : blob_items) {
        hash_temp = blob_item.substr(0, HASH_LEN);
        path_temp = blob_item.substr(HASH_LEN);
        blob_temp = Blob::get_blob_from_db(hash_temp);
        // check path correctness
        if (blob_temp->get_path() != path_temp) {
            cerror::occur_error("Blob hash matching failed. Hash is " +
                                hash_temp);
        }
    #ifdef DIR_TEST
        cout << "load blob: " << blob_item << endl << path_temp << endl;
    #endif
        dir.files.push_back(std::move(blob_temp));
    }
    f.close();
}

void Dir::recover_dir() {
    for (Blob *blob : files) {
        blob->save_as_file();
    #ifdef DEBUG
        std::cout << blob->get_path() << std::endl;
    #endif
    }
    for (Dir *dir : dirs) {
        util::cmkdir(dir->get_path(), dir->permission);
        dir->recover_dir();
    #ifdef DEBUG
        std::cout << dir->get_path() << std::endl;
    #endif
    }
}

Dir* Dir::get_dir_from_path(const std::string &path) {
    using namespace std;
    Dir *res = new Dir;
    res->init_hdr();
    res->path = path;
    res->hdr.path_len = path.length();
    res->permission = util::get_dir_permission(path);
    res->hash = sha1::hash_a_string(path);
    #ifdef DEBUG
    cout << "new a Dir: " << path << "  " << res->permission << endl;
    #endif
    vector<string> files_temp = util::get_all_files(path);
    vector<string> dirs_temp = util::get_all_dirs(path);
    if (files_temp.size() == 0) {
        #ifdef DEBUG
        cout << "no files" << endl;
        #endif
    } else if (dirs_temp.size() == 0 && files_temp.size() == 0) {
        #ifdef DEBUG
        cout << "empty dir: " << path << endl;
        #endif
        // TODO empty judge
        return res;
    }
    // get all files in this path, and generate Blob objects
    res->files.reserve(files_temp.size());
    for (std::string file_str : files_temp) {
        res->files.push_back(std::move(Blob::get_blob_from_file(path + "/" + file_str)));
    }

    // get all dirs in this path, and generate Dir objects
    res->dirs.reserve(dirs_temp.size());
    for (std::string dir_str : dirs_temp) {
        #ifdef DEBUG
        std::cout<<"dir: "<< dir_str<<std::endl;
        #endif
        res->dirs.push_back(std::move(Dir::get_dir_from_path(path + "/" + dir_str)));
    }

    #ifdef DIR_TEST
    res->files_names = files_temp;
    res->dirs_names = dirs_temp;
    #endif
    return res;
}

Dir::~Dir() {
    using namespace std;
    #ifdef DEBUG
    std::cout << "delete a dir "<< path << std::endl;
    #endif
    for (auto it = files.begin(); it != files.end(); it++) {
        delete *it;
    }
    for (auto it = dirs.begin(); it != dirs.end(); it++) {
        delete *it;
    }
}

Dir::Dir(Dir&& dir) {
    permission = dir.permission;
    path = std::move(dir.path);
    files = std::move(dir.files);
    dirs = std::move(dir.dirs);
    hdr = std::move(dir.hdr);
}

#ifdef DIR_TEST

int compare_files(Dir &dir) {
    using namespace std;
    if (dir.files_names.size() != dir.get_files().size()) {
        cout << "Size not correct" << endl;
        cout << "names num: " << dir.files_names.size() << endl;
        cout << "blob num: " << dir.get_files().size() << endl;
        return 0;
    }
    int flag = 1;
    auto blob_it = dir.get_files().begin();
    auto name_it = dir.files_names.begin();
    while (name_it != dir.files_names.end() &&
           blob_it != dir.get_files().end()) {
        string name1 = *name_it;
        const string name2 = (*blob_it)->get_path();
        if (name1 != name2) {
            flag = 0;
            cout << "name compare failed" << endl;
            cout << "from name: " << name1 << endl;
            cout << "from blob: " << name2 << endl;
        } else {
            cout << "pass: " << name1 << endl;
            cout << "size is ";
            int size = (*blob_it)->get_hdr().size;
            cout << size << endl;
        }
        name_it++;
        blob_it++;
    }
    return flag;
}
#include <memory>
int main(int args, char **argv) {
    if (args < 2) {
        std::cout << "no path!!" << std::endl;
    }
    std::string obj_path = string(argv[1]) + string(".obj");
    
    std::unique_ptr<Dir> root(Dir::get_dir_from_path(argv[1]));
    root->save_as_obj(obj_path);
    
    /*
    std::unique_ptr<Dir> root(Dir::get_dir_from_obj(obj_path));
    root->recover_dir();
    */
    return 0;
}
#endif
