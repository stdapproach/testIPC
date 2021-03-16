#ifndef LIB1_H
#define LIB1_H

#include "lib_global.h"
#include "types.h"

extern "C" {
LIB1_EXPORT int foo();
LIB1_EXPORT types::Str handlerName();
LIB1_EXPORT int handlerFunc(types::Str str);
}

#endif // LIB1_H
