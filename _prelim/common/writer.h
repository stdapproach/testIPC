#ifndef WRITER_H
#define WRITER_H

#include "writeHelper.h"
#include "memHelper.h"

namespace sm {
using storageDesr_t = memHelper::storageDesr_t;
using storage_size_t = memHelper::storage_size_t;
using descr_t = memHelper::descr_t;
using res_t = memHelper::res_t;

struct Writer final : memHelper::base {
    Writer() = delete;
    Writer(storageDesr_t storageID, storage_size_t size)
        :base(storageID, size) {
        const auto res{openRW()};
        if(res.hasError()) {
            errHandler(res);
        } else {
            const auto res{extentSN()};
            if(res.hasError()) {
                errHandler(res);
            } else {
                const auto res{mapM()};
                if(res.hasError()) {
                    errHandler(res);
                }
            }
        }
    }
    void write(std::string data) const{
        if (!_ready) {
            throw("impossible write to non-ready memory");
        }
        writeHelper::copyToSM(data, _mem);
    }
    ~Writer() {
        try {
            const auto res{cleanup()};
            if(res.hasError()) {
                errHandler(res);
            } else {
                const auto res{unlink()};
                if(res.hasError()) {
                    errHandler(res);
                }
            }
            if(_lastError.has()) {
                std::cerr << "~Writer() Error: " <<_lastError.descr.c_str() << std::endl;
            }

        }  catch (...) {
            std::cerr << "Writer::~Writer exception raised\n";
        }
    }

private:
    res_t openRW() {
        auto res{writeHelper::openRW(_storageID)};
        _resOpening = res.val;
        return res;
    }
    res_t extentSN() const{
        auto res{writeHelper::extentSN(_resOpening, _size)};
        return res;
    }
    resHelper::res_t<descr_t> mapM() {
        auto res{writeHelper::memMappingWrite(_size, _resOpening)};
        _ready = !res.hasError();
        if(_ready) {
            _mem = res.val;
        }
        return res;
    }
    res_t cleanup() {
        auto res{writeHelper::mapCleanup(_mem, _size)};
        if(res.hasError()) {
            const auto& err = res.error;
            perror(err.descr.c_str());
            return err.code;
        }
        return res;
    }
    res_t unlink() {
        auto res{writeHelper::unlink(_storageID)};
        if(res.hasError()) {
            const auto& err = res.error;
            perror(err.descr.c_str());
            return err.code;
        }
        return res;
    }
};

}

#endif // WRITER_H
