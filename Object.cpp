#include "Object.hpp"

void Object::init_hdr() {
    hdr.version = VERSION;
    hdr.signature = SIGNATURE;
    hdr.time = time(NULL);
}

obj_hdr_t& Object::get_hdr() {
    return hdr;
}

std::string& Object::get_hash() {
    return hash;
}

int Object::load_hdr(std::ifstream& f) {
    f.read((char*)(&hdr), sizeof(obj_hdr_t));
#ifdef DEBUG
    cout << hdr.size << endl;
#endif
    if (hdr.signature != SIGNATURE) {
        return -1;
    }
    return 0;
}