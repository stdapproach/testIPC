#ifndef WRITEHELPER_H
#define WRITEHELPER_H

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include <string.h>

#include "memHelper_t.h"

namespace writeHelper {
using storageDesr_t = memHelper::storageDesr_t;
using storage_size_t = memHelper::storage_size_t;
using descr_t = memHelper::descr_t;
using res_t = memHelper::res_t;

/*
 * get shared memory file descriptor (NOT a file)
 * params: descr - name of shared memory
 */
res_t openRW(storageDesr_t descr) {
    auto fd = shm_open(descr, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if(-1 != fd) {
        return {fd};
    }
    static const auto defErroResult{10};
    return {resHelper::err_t{defErroResult, "open wrong"}};
}

/*
 * extend shared memory object as by default it's initialized with size 0
 * params: openResult - result of [openRW], storageSize - size,
 */
res_t extentSN(res_t::val_t openResult, paramComm::storage_size_t newSize) {
    auto res = ftruncate(openResult, newSize);
    if (-1 != res) {
        return {res};
    }
    static const auto defErroResult{20};
    return {resHelper::err_t{defErroResult, "ftruncate wrong"}};
}

/*
 * map shared memory to process address space
 * params: storageSize - size, openResult - result of [openRW]
 */
resHelper::res_t<descr_t> memMappingWrite(storage_size_t storageSize, res_t::val_t openResult) {
    descr_t res = mmap(nullptr, storageSize, PROT_WRITE, MAP_SHARED, openResult, 0);
    if (res == MAP_FAILED)
    {
        static const auto defErroResult{30};
        return {resHelper::err_t{defErroResult, "mmap wrong"}};
    }
    return {res};
}

/*
 * place data into memory
 * params: data - source string, descr - name of shared memory
 */
void copyToSM(std::string data, descr_t descr) {
    memcpy(descr, data.c_str(), data.length());
}

/*
 * mmap cleanup
 * params: descr - name of shared memory, storageSize - size
 */
res_t mapCleanup(descr_t descr, storage_size_t storageSize) {
    auto res = munmap(descr, storageSize);
    if (res == -1)
    {
        static const auto defErroResult{40};
        return {resHelper::err_t{defErroResult, "munmap wrong"}};
    }
    return {res};
}

/*
 * shm_open cleanup
 * params: descr - name of shared memory
 */
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

#endif // WRITEHELPER_H
