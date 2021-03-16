#ifndef LIBHANDLER_H
#define LIBHANDLER_H

#include <iostream>
#include <memory>
#include "osSelector.h"
#include "types.h"

#ifdef OSSELECTOR_POSIX
//
#elif defined(OSSELECTOR_WIN)
    #include "libHandler_Win.h"
#endif

namespace libHandler{
using namespace types;

using res_t = lib::helperT<helper::osSelector()>::res_t;

/*
 * crossplatfrom function to load dll/so
 */
void* loadLib(Str name)
{
    auto res = lib::loadLibImpl<res_t>(name);
    if (nullptr == res) {
        std::cout << "cannot locate the .dll/.so file=" << name << std::endl;
    } else {
        std::cout << "it has been loaded" << std::endl;
    }
    return static_cast<void*>(res);
}

bool unloadLib(void* h)
{
    auto res = lib::unloadLibImpl<res_t>(h);
    if (res) {
        std::cout << ".dll/.so unloaded SUCCESS\n";
    } else {
        std::cout << ".dll/.so unloaded WRONG\n";
    }
    return res;
}

template<typename F>
F getFunc(void* handler, types::Str nameF) {
    return lib::getLibFunctionImpl<F>(handler, nameF);
}

struct dlib {
    dlib() noexcept = delete;
    dlib(dlib const&) noexcept = default;
    dlib(dlib &&) noexcept =default;
    dlib& operator=(dlib const&) noexcept = default;
    dlib& operator=(dlib &&) noexcept = default;
    explicit operator bool() const { return static_cast<bool>(_handle); }
    dlib(types::Str name) try :_name{name} {
        std::cout << "start load dlib " << name << std::endl;
        void* h = loadLib(name);
        if (h) {
            _handle = std::shared_ptr<void>(
                h,
                [name](void* h) {
                    std::cout << "unload dlib " << name << std::endl;
                    unloadLib(h);
                }
            );
        }
        std::cout << "finish load Dll/so " << name << std::endl;
    }
    catch(const std::exception& e) {
        std::cout << "dlib, exception in Ctr: " << e.what() << std::endl;
        throw;
    }
    catch(...) {
        std::cout << "dlib, unknown exception" << std::endl;
        throw;
    }
    ~dlib() = default;
    void* handle(){return _handle.get();}
    types::Str name()const{return _name;};
private:
    std::shared_ptr<void> _handle;
    types::Str _name;
};

}

#endif // LIBHANDLER_H