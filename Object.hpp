#ifndef OBJECT_H
#define OBJECT_H
#include <ctime>
#include <string>
#define SIGNATURE 0x1789
#define VERSION 0x99
using namespace std;

struct obj_hdr_t {
    int version;
    int signature;
    int size;
    int path_len;
    time_t time;
};

class Object {
   private:
   protected:
    struct obj_hdr_t hdr;
    string hash;
    void init_hdr();

   public:
    virtual void save_as_obj(string obj_path) = 0;
    obj_hdr_t& get_hdr();
    string& get_hash();
};

#endif