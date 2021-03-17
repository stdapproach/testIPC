#ifndef READHELPER_H
#define READHELPER_H

#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "memHelper_t.h"

namespace readHelper {
using storageDesr_t = memHelper::storageDesr_t;
using storage_size_t = memHelper::storage_size_t;
using descr_t = memHelper::descr_t;
using res_t = memHelper::res_t;

/*
 * get shared memory file descriptor (NOT a file)
 * params: descr - name of shared memory
 */
res_t openReadOnly(storageDesr_t descr) {
    auto fd = shm_open(descr, O_RDONLY, S_IRUSR | S_IWUSR);
    if(-1 != fd) {
        return {fd};
    }
    static const auto defErroResult{10};
    return {resHelper::err_t{defErroResult, "open wrong"}};
}

/*
 * map shared memory to process address space
 * params: storageSize - size, openResult - result of [openReadOnly]
 */
resHelper::res_t<descr_t> memMappingRead(storage_size_t storageSize, res_t::val_t openResult) {
    descr_t res = mmap(nullptr, storageSize, PROT_READ, MAP_SHARED, openResult, 0);
    if (res == MAP_FAILED)
    {
        static const auto defErroResult{30};
        return {resHelper::err_t{defErroResult, "mmap wrong"}};
    }
    return {res};
}

/*
 * place data into memory
 * params: descr - name of shared memory
 * params: storageSize - size
 * params: buff - buffer with appropriate size
 */
void copyFromSM(descr_t descr, storage_size_t storageSize, char* buff) {
    memcpy(buff, descr, storageSize);
}

}
#endif // READHELPER_H
