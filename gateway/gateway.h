#ifndef GATEWAY_H
#define GATEWAY_H

#include <type_traits>
#include <map>

namespace gateaway {
using strT = std::string;

struct handlerRegistr {
    using key_t = const char* ;
    using buf_t = char** ;

    using fName = key_t(*)();
    using Func = int (*)(key_t str, buf_t);
    void reg(key_t name, Func func) {
        _map[name] = func;
    }
    void unreg(key_t name) {
        auto iter{_map.find(name)};
        if (iter != _map.end()) {
            _map.erase(iter);
        }
    }
private:
    std::map<key_t, Func> _map;
};
}

#endif // GATEWAY_H
