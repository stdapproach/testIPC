#ifndef LIBHANDLER_NIX_H
#define LIBHANDLER_NIX_H

#include "libHandlerCommon.h"
#include <iostream>
#include <string>

/*
 * dLib handler for Linux
 */
namespace lib {
using strT = std::string;

constexpr auto os{helper::osSelector()};
#ifdef OSSELECTOR_POSIX
//
    #include <dlfcn.h>
    template<>
    struct helperT<os> {
        typedef void* res_t;
    };

    using res_t = helperT<os>::res_t;
    template<>
    auto loadLibImpl<res_t>(strT name)->res_t
    {
        res_t hGetProcIDDLL = dlopen(name.c_str(), RTLD_LAZY);
        dlerror();
        return static_cast<res_t>(hGetProcIDDLL);
    }

    template<>
    bool unloadLibImpl<res_t>(res_t handler)
    {
        bool res{true};
        if (nullptr != handler)
        {
            res = dlclose(handler) == 0;
        }
        return res;
    }

    template<typename F>
    F getLibFunctionImpl(res_t handler, strT nameF)
    {
        auto defResult{nullptr};
        if(!handler) {
            std::cout << "it's impossible to get function from nullptr\n";
            return defResult;
        }

        auto raw = dlsym(handler, nameF.c_str());
        if(!raw) {
            std::cout << "there is no function with name " << nameF.c_str() << std::endl;
            return defResult;
        }
        auto res = reinterpret_cast<F>(raw);
        if (!res){
            std::cout << "wrong cast to function with name " << nameF.c_str() << std::endl;
            return defResult;
        }
        return res;
    }

#elif defined(OSSELECTOR_WIN)
#endif
}

#endif // LIBHANDLER_NIX_H
