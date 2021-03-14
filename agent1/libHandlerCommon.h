#ifndef LIHANDLERCOMMON_H
#define LIHANDLERCOMMON_H

#include "osSelector.h"
#include "types.h"

namespace lib {
/*
 * common function to load dll/so
 */
template<helper::supportedOS os>
void* loadLibImpl(types::Str name);

/*
 * common function to unload dll/so
 */
template<helper::supportedOS os>
bool unloadLibImpl(void* handler);

}

#endif // LIHANDLERCOMMON_H
