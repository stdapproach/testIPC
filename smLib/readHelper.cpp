#include "readHelper.h"

namespace readHelper {
res_t openReadOnly(storageDesr_t descr) {
    auto fd = shm_open(descr, O_RDONLY, S_IRUSR | S_IWUSR);
    if(-1 != fd) {
        return {fd};
    }
    static const auto defErroResult{10};
    return {resHelper::err_t{defErroResult, "open wrong"}};
}

resHelper::res_t<descr_t> memMappingRead(storage_size_t storageSize, res_t::val_t openResult) {
    descr_t res = mmap(nullptr, storageSize, PROT_READ, MAP_SHARED, openResult, 0);
    if (res == MAP_FAILED)
    {
        static const auto defErroResult{30};
        return {resHelper::err_t{defErroResult, "mmap wrong"}};
    }
    return {res};
}

void copyFromSM(descr_t descr, storage_size_t storageSize, char* buff) {
    memcpy(buff, descr, storageSize);
}

}
