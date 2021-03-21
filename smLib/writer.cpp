#include "writer.h"

namespace sm {

Writer::Writer(storageDesr_t storageID, storage_size_t size)
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

void Writer::write(std::string data) const{
    if (!_ready) {
        throw("impossible write to non-ready memory");
    }
    writeHelper::copyToSM(data, _mem);
}

Writer::~Writer() {
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

res_t Writer::openRW() {
    auto res{writeHelper::openRW(_storageID)};
    _resOpening = res.val;
    return res;
}

res_t Writer::extentSN() const{
    auto res{writeHelper::extentSN(_resOpening, _size)};
    return res;
}

resHelper::res_t<descr_t> Writer::mapM() {
    auto res{writeHelper::memMappingWrite(_size, _resOpening)};
    _ready = !res.hasError();
    if(_ready) {
        _mem = res.val;
    }
    return res;
}

res_t Writer::cleanup() {
    auto res{writeHelper::mapCleanup(_mem, _size)};
    if(res.hasError()) {
        const auto& err = res.error;
        perror(err.descr.c_str());
        return err.code;
    }
    return res;
}

res_t Writer::unlink() {
    auto res{writeHelper::unlink(_storageID)};
    if(res.hasError()) {
        const auto& err = res.error;
        perror(err.descr.c_str());
        return err.code;
    }
    return res;
}

}
