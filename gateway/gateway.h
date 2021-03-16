#ifndef GATEWAY_H
#define GATEWAY_H

#include <type_traits>
#include <map>
#include "types.h"

namespace gateaway {

struct handlerRegistr {
    using key_t = types::Str;

    using fName = key_t(*)();
    using Func = int (*)(key_t str);
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