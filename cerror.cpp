#include "cerror.hpp"
#include <stdlib.h>

using namespace std;

void cerror::occur_error(const char* info) {
    cerr << "Error: " << info << endl;
    exit(1);
}
