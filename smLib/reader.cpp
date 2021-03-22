#include "reader.h"

namespace sm {
Reader::Reader(storageDesr_t storageID, storage_size_t size)
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

std::string Reader::read() const {
    if (!_ready) {
        throw("impossible read from non-ready memory");
    }
    std::unique_ptr<char[]> ptr(new char[_size]);
    readHelper::copyFromSM(_mem, _size, ptr.get());
    std::string res(&ptr[0], &ptr[0] + _size);
    return res;
}

Reader::~Reader() {
    if(_lastError.has()) {
        std::cerr << "~Reader() Error: " << _lastError.descr.c_str() << std::endl;
    }
}

res_t Reader::openRO() {
    auto res{readHelper::openReadOnly(_storageID)};
    _resOpening = res.val;
    return res;
}

resHelper::res_t<descr_t> Reader::mapM() {
    auto res{readHelper::memMappingRead(_size, _resOpening)};
    _ready = !res.hasError();
    if(_ready) {
        _mem = res.val;
    }
    return res;
}

}
