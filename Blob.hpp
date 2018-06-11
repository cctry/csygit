#ifndef BLOB_H
#define BLOB_H

#include "Object.hpp"
class Blob: public Object
{
private:
    string path;
    string content;
    int permission;
public:
    static Blob* get_blob_from_file(std::string path);
    static Blob* get_blob_from_db(std::string hash);
    virtual void save_as_obj(string obj_path);
    void save_as_file();
    string& get_content();
    string& get_path();
    int get_permission();
    #ifdef BLOB_TEST
    static Blob* load_obj(string obj_path);
    #endif
};

#endif