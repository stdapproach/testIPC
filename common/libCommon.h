#ifndef LIB1_H
#define LIB1_H

#include "lib_global.h"

extern "C" {
LIB1_EXPORT const char* handlerName();
LIB1_EXPORT int handlerFunc(const char* str, char** buffer);
}

#endif // LIB1_H
