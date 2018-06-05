#include "Object.hpp"

void Object::init_hdr() {
    hdr.version = VERSION;
    hdr.signature = SIGNATURE;
    hdr.time = time(NULL);
}

obj_hdr_t& Object::get_hdr() {
    return hdr;
}