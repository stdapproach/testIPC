#ifndef LIHANDLERCOMMON_H
#define LIHANDLERCOMMON_H

#include <string>
#include "osSelector.h"

namespace lib {
using strT = std::string;
/*
 * common function to load dll/so
 */
template<typename U>
U loadLibImpl(strT name);

/*
 * common function to unload dll/so
 */
template<typename T>
bool unloadLibImpl(T handler);

/*
 * helper for manipulate by types depending OS's type
 */
template<helper::supportedOS os>
struct helperT;

}

#endif // LIHANDLERCOMMON_H
