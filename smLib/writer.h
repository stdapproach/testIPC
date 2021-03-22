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
    Writer(const Writer&) = delete;
    Writer& operator=(Writer&) = delete;
    /*
     * ctr
     * params: name and size of SM
     */
    Writer(storageDesr_t storageID, storage_size_t size, std::string internalID, bool unlinkRequired = true);

    /*
     * place data into memory
     * params: data - source string
     */
    void write(std::string data) const;

    ~Writer();

    std::string read() const;

private:
    std::string _internalID;
    void debugPrintout(std::string postfix)const;
    bool _unlinkRequired;
    /*
     * open shared memory (read/write)
     */
    res_t openRW();

    /*
     * extend shared memory object as by default it's initialized with size 0
     */
    res_t extentSN() const;

    /*
     * map shared memory to process address space
     */
    resHelper::res_t<descr_t> mapM();

    /*
     * mmap cleanup
     */
    res_t cleanup();

    /*
     * shm_open cleanup
     */
    res_t unlink();
};

}

#endif // WRITER_H
