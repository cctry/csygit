#pragma once
#include "Object.hpp"
class Blob : public Object {
   private:
    string path;
    string content;
    int permission;

   public:
    static Blob* get_blob_from_file(const std::string& path);
    static Blob* get_blob_from_db(const std::string& hash);
    virtual void save_as_obj(const std::string& obj_path);
    void save_as_file();
    Blob(Blob&& blob);
    const std::string& get_content() { return content; }
    const std::string& get_path() { return path; }
    const int get_permission() { return permission; }
    Blob() {};
#ifdef BLOB_TEST
    static Blob* load_obj(const std::string& obj_path);
#endif
};
