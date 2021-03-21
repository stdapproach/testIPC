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
res_t openRW(storageDesr_t descr);

/*
 * extend shared memory object as by default it's initialized with size 0
 * params: openResult - result of [openRW], storageSize - size,
 */
res_t extentSN(res_t::val_t openResult, storage_size_t newSize);

/*
 * map shared memory to process address space
 * params: storageSize - size, openResult - result of [openRW]
 */
resHelper::res_t<descr_t> memMappingWrite(storage_size_t storageSize, res_t::val_t openResult);

/*
 * place data into memory
 * params: data - source string, descr - name of shared memory
 */
void copyToSM(std::string data, descr_t descr);

/*
 * mmap cleanup
 * params: descr - name of shared memory, storageSize - size
 */
res_t mapCleanup(descr_t descr, storage_size_t storageSize);

/*
 * shm_open cleanup
 * params: descr - name of shared memory
 */
res_t unlink(storageDesr_t descr);

}

#endif // WRITEHELPER_H
