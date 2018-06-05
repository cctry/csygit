#ifndef CERROR_H
#define CERROR_H

#include <error.h>
#include <stdlib.h>
#include <iostream>

using namespace std;
namespace cerror {
    
void occur_error(const char* info);

}  // namespace cerror

#endif