#ifndef MEMHELPER_H
#define MEMHELPER_H

#include <iostream>
#include "memHelper_t.h"

namespace memHelper {

struct base {
    base() = delete;
    base(storageDesr_t storageID, storage_size_t size)
        :_storageID{storageID}, _size{size}, _lastError{}
    {}
protected:
    int a;
    const storageDesr_t _storageID;
    const storage_size_t _size;

    template<typename T>
    void errHandler(T res) {
        const auto& err = res.error;
        _lastError = err;
    }
    static const int unit{-1};
    resHelper::err_t _lastError;
    res_t::val_t _resOpening{unit};
    bool _ready{false};
    descr_t _mem{nullptr};
};

}

#endif // MEMHELPER_H
