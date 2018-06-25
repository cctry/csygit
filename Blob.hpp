#ifndef BLOB_H
#define BLOB_H

#include "Object.hpp"
class Blob : public Object {
   private:
    string path;
    string content;
    int permission;

   public:
    static Blob* get_blob_from_file(const std::string& path);
    static Blob* get_blob_from_db(const std::string& hash);
    virtual void save_as_obj(string obj_path);
    void save_as_file();
    const string& get_content() { return content; }
    const string& get_path() { return path; }
    const int get_permission() { return permission; }
#ifdef BLOB_TEST
    static Blob* load_obj(string obj_path);
#endif
};

#endif