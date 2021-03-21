#include "writeHelper.h"

namespace writeHelper {

res_t openRW(storageDesr_t descr) {
    auto fd = shm_open(descr, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if(-1 != fd) {
        return {fd};
    }
    static const auto defErroResult{10};
    return {resHelper::err_t{defErroResult, "open wrong"}};
}

res_t extentSN(res_t::val_t openResult, storage_size_t newSize) {
    auto res = ftruncate(openResult, newSize);
    if (-1 != res) {
        return {res};
    }
    static const auto defErroResult{20};
    return {resHelper::err_t{defErroResult, "ftruncate wrong"}};
}

resHelper::res_t<descr_t> memMappingWrite(storage_size_t storageSize, res_t::val_t openResult) {
    descr_t res = mmap(nullptr, storageSize, PROT_WRITE, MAP_SHARED, openResult, 0);
    if (res == MAP_FAILED)
    {
        static const auto defErroResult{30};
        return {resHelper::err_t{defErroResult, "mmap wrong"}};
    }
    return {res};
}

void copyToSM(std::string data, descr_t descr) {
    memcpy(descr, data.c_str(), data.length());
}

res_t mapCleanup(descr_t descr, storage_size_t storageSize) {
    auto res = munmap(descr, storageSize);
    if (res == -1)
    {
        static const auto defErroResult{40};
        return {resHelper::err_t{defErroResult, "munmap wrong"}};
    }
    return {res};
}

res_t unlink(storageDesr_t descr) {
    int res = shm_unlink(descr);
    if (res == -1)
    {
        static const auto defErroResult{100};
        return {resHelper::err_t{defErroResult, "unlink wrong"}};
    }
    return {res};
}

}

