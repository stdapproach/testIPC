#ifndef ENVHELPER_H
#define ENVHELPER_H

#include <vector>
#include <string>

#include "resHelper.h"

namespace env {
using TStr = std::string;

std::vector<TStr> toVector(int argc, char *argv[]) {
    std::vector<TStr> res;
    for (auto i = 0; i< argc; ++i) {
        res.emplace_back(argv[i]);
    }
    return res;
}

resHelper::res_t<TStr> getNonEmptyString(uint index, std::vector<TStr> params) {
    if (index >= params.size()) {
        return resHelper::err_t{1, "index out of bound"};
    }
    auto res = params[index];
    if (res.empty()) {
        return resHelper::err_t{2, "empty string"};
    }
    return {res};
}

}

#endif // ENVHELPER_H
