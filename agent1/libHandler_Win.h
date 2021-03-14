#ifndef LIBHANDLER_WIN_H
#define LIBHANDLER_WIN_H

#include "libHandlerCommon.h"

namespace lib {

#ifdef OSSELECTOR_POSIX
//
#elif defined(OSSELECTOR_WIN)
    #include <windows.h>

    template<>
    void* loadLibImpl<helper::osSelector()>(types::Str name)
    {
        HINSTANCE hGetProcIDDLL = LoadLibrary(name);
        return hGetProcIDDLL;
    }

    template<>//QQQ
    bool unloadLibImpl<helper::osSelector()>(void* handler)
    {
        bool fFreeResult{true};
        if (nullptr != handler)
        {
           fFreeResult = FreeLibrary((HINSTANCE)handler);
        }
        return fFreeResult;
    }
#endif
}

#endif // LIBHANDLER_WIN_H
