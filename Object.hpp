#pragma once
#include <ctime>
#include <string>
#include <fstream>
#define SIGNATURE 0x1789
#define VERSION 0x99
using namespace std;

struct obj_hdr_t {
    int version;
    int signature;
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
    virtual void save_as_obj(const string& obj_path) = 0;
    obj_hdr_t& get_hdr();
    const string& get_hash() {return hash;};
    int load_hdr(ifstream& f);
    #ifdef DEBUG
    void prtHdr();
    #endif
};
