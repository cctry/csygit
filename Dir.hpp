#pragma once
#include <string>
#include <vector>
#include "Blob.hpp"
#include "Object.hpp"

class Dir : public Object {
   public:
    virtual void save_as_obj(const std::string& obj_path);
    static Dir* get_dir_from_path(const std::string& path);
    static Dir* get_dir_from_db(const std::string& hash);
    const std::string& get_path() { return path; }
    const int get_permission() { return permission; }
    void recover_dir();
    static void load_obj(const std::string& obj_path, Dir& dir);
    Dir() {};
    ~Dir();
    Dir(Dir&& dir);
#ifdef DIR_TEST
    const std::vector<Blob*>& get_files() { return files; }
    const std::vector<Dir*>& get_dirs() { return dirs; }
    std::vector<std::string> files_names;
    std::vector<std::string> dirs_names;
#endif
   private:
    int permission;
    std::string path;
    std::vector<Blob*> files;
    std::vector<Dir*> dirs;
};
