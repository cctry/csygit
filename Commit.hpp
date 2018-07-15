#pragma once
#include <string>
#include "Blob.hpp"
#include "Dir.hpp"
#include "Object.hpp"

class Commit : public Object {
   private:
    std::string content;
    std::string pre_hash;
    std::string next_hash;
    Dir* dir;

   public:
    Commit(){};
    ~Commit();
    virtual void save_as_obj(const std::string& obj_path);
    static Commit* get_from_dir(const std::string& path);
    static Commit* get_from_db(const std::string& hash);
	std::string& set_content() {return content;};
	const std::string& get_content() {return content;};
	const std::string& get_prev() {return pre_hash;};
	const std::string& get_next() {return next_hash;};
	void print();
};
