#pragma once
#include <ctime>
#include <fstream>
#include <string>
#define SIGNATURE 0x1789
#define VERSION 0x99

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
    std::string hash;
    void init_hdr();

   public:
    virtual void save_as_obj(const std::string& obj_path) = 0;
    obj_hdr_t& get_hdr();
    const std::string& get_hash() { return hash; };
    int load_hdr(std::ifstream& f);
    virtual ~Object(){};
#ifdef DEBUG
    void prtHdr();
#endif
};
