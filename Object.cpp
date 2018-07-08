#include "Object.hpp"
#include <iostream>
#include <string>

void Object::init_hdr() {
    hdr.version = VERSION;
    hdr.signature = SIGNATURE;
    hdr.time = time(NULL);
}

#ifdef DEBUG
void Object::prtHdr() {
    std::cout << "Signature: " << hdr.signature << std::endl
              << "Version: " << hdr.version << std::endl
              << "Path_len: " << hdr.path_len << std::endl;
}
#endif

obj_hdr_t& Object::get_hdr() {
    return hdr;
}

int Object::load_hdr(std::ifstream& f) {
    f.read((char*)(&hdr), sizeof(obj_hdr_t));
    if (hdr.signature != SIGNATURE) {
        return -1;
    }
    return 0;
}