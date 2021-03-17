#ifndef READER_H
#define READER_H

#include <memory>

#include "readHelper.h"
#include "memHelper.h"

namespace sm {
using storageDesr_t = memHelper::storageDesr_t;
using storage_size_t = memHelper::storage_size_t;
using descr_t = memHelper::descr_t;
using res_t = memHelper::res_t;

/*
 * To read data from SharedMemory
 * using: ctr with name and size
 * than: invoke method read()
 */
struct Reader : memHelper::base  {
    Reader() = delete;
    Reader(storageDesr_t storageID, storage_size_t size)
        :base(storageID, size) {
        const auto res{openRO()};
        if(res.hasError()) {
            errHandler(res);
        } else {
            const auto res{mapM()};
            if(res.hasError()) {
                errHandler(res);
            }
        }
    }
    std::string read() const {
        if (!_ready) {
            throw("impossible read from non-ready memory");
        }
        std::unique_ptr<char[]> ptr(new char[_size]);
        readHelper::copyFromSM(_mem, _size, ptr.get());
        std::string res(&ptr[0], &ptr[0] + _size);
        return res;
    }
    ~Reader() {
        if(_lastError.has()) {
            std::cerr << "~Reader() Error: " <<_lastError.descr.c_str() << std::endl;
        }
    }

private:
    res_t openRO() {
        auto res{readHelper::openReadOnly(_storageID)};
        _resOpening = res.val;
        return res;
    }
    resHelper::res_t<descr_t> mapM() {
        auto res{readHelper::memMappingRead(_size, _resOpening)};
        _ready = !res.hasError();
        if(_ready) {
            _mem = res.val;
        }
        return res;
    }

    descr_t _mem{nullptr};
};

}
#endif // READER_H
