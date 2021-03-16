#ifndef LIBHANDLER_WIN_H
#define LIBHANDLER_WIN_H

#include "libHandlerCommon.h"
#include <iostream>

namespace lib {

#ifdef OSSELECTOR_POSIX
//
#elif defined(OSSELECTOR_WIN)
    #include <windows.h>

    constexpr auto os{helper::osSelector()};

    template<>
    struct helperT<os> {
        typedef HINSTANCE libRaw_t;
        typedef void* res_t;
    };

    using res_t = helperT<os>::res_t;
    using libRaw_t = helperT<os>::libRaw_t;

    template<>
    auto loadLibImpl<res_t>(types::Str name)->res_t
    {
        libRaw_t hGetProcIDDLL = LoadLibrary(name);
        return static_cast<res_t>(hGetProcIDDLL);
    }

    template<>
    bool unloadLibImpl<res_t>(res_t handler)
    {
        bool fFreeResult{true};
        if (nullptr != handler)
        {
            auto ptr = static_cast<libRaw_t>(handler);
            if (!ptr) {
                return false;
            }
            fFreeResult = FreeLibrary(ptr);
        }
        return fFreeResult;
    }

    template<typename F>
    F getLibFunctionImpl(res_t handler, types::Str nameF)
    {
        auto defResult{nullptr};
        if(!handler) {
            std::cout << "it's impossible to get function from nullptr\n";
            return defResult;
        }
        auto raw = GetProcAddress(static_cast<libRaw_t>(handler), nameF);
        if(!raw) {
            std::cout << "there is no function with name " << nameF << std::endl;
            return defResult;
        }
        auto res = reinterpret_cast<F>(raw);
        if (!res){
            std::cout << "wrong cast to function with name " << nameF << std::endl;
            return defResult;
        }
        return res;
    }
#endif
}

#endif // LIBHANDLER_WIN_H
