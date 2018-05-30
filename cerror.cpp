#include "cerror.hpp"
#include <stdlib.h>

using namespace std;

void occur_error(const char* info) {
    cerr << "Error: " << info << endl;
    exit(1);
}
