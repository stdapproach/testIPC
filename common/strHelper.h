#ifndef STRHELPER_H
#define STRHELPER_H

#include <string>
#include <iostream>
#include <string.h>

namespace helper {
/*
 * convinient wrapper for copying string to char*
 */
int toBuff(std::string orig, char** dest /*nullptr*/) {
    using namespace std;
    *dest = new char[orig.size()+1];
    strcpy(*dest, orig.c_str());
    return 0;
}

}

#endif // STRHELPER_H
