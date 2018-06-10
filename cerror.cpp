#include "cerror.hpp"
#include <stdlib.h>

using namespace std;

void cerror::occur_error(string info) {
    cerr << "Error: " << info << endl;
    exit(1);
}
