#ifndef LIBHANDLER_H
#define LIBHANDLER_H

#include <iostream>
#include <memory>
#include "osSelector.h"

#ifdef OSSELECTOR_POSIX
    #include "libHandler_Nix.h"
#elif defined(OSSELECTOR_WIN)
    #include "libHandler_Win.h"
#endif

namespace libHandler{
using strT = std::string;
using res_t = lib::helperT<helper::osSelector()>::res_t;

/*
 * crossplatfrom function to load dll/so
 */
void* loadLib(strT name)
{
    auto res = lib::loadLibImpl<res_t>(name);
    return static_cast<void*>(res);
}

bool unloadLib(void* h)
{
    auto res = lib::unloadLibImpl<res_t>(h);
    if (!res) {
        std::cout << "dLib unloaded WRONG\n";
    }
    return res;
}

template<typename F>
F getFunc(void* handler, strT nameF) {
    return lib::getLibFunctionImpl<F>(handler, nameF);
}

struct dlib {
    dlib() noexcept = delete;
    dlib(dlib const&) noexcept = default;
    dlib(dlib &&) noexcept =default;
    dlib& operator=(dlib const&) noexcept = default;
    dlib& operator=(dlib &&) noexcept = default;
    explicit operator bool() const { return static_cast<bool>(_handle); }
    dlib(strT name) try :_name{name} {
        void* h = loadLib(name);
        if (h) {
            _handle = std::shared_ptr<void>(
                h,
                [name](void* h) {
                    unloadLib(h);
                }
            );
        }
    }
    catch(const std::exception& e) {
        std::cout << "dlib, exception in Ctr: " << e.what() << std::endl;
        throw;
    }
    catch(...) {
        std::cout << "dlib, unknown exception\n";
        throw;
    }
    ~dlib() = default;
    void* handle(){return _handle.get();}
    strT name()const{return _name;};
private:
    std::shared_ptr<void> _handle;
    strT _name;
};

template<helper::supportedOS os>
struct helperT;

template<>
struct helperT<helper::supportedOS::posix> {
    const strT prefix{"./"};
    const strT prefixLib{"lib"};
    const strT postfixDLib{".so"};
    const strT postfixExec{""};
    strT getFullExecName(strT rawName) {
        return prefix + rawName + postfixExec;
    }
    strT getFullDLibName(strT rawName) {
        return prefix + rawName + postfixDLib;
    }
};

}

#endif // LIBHANDLER_H
