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

    /*
     * ctr
     * params: name and size of SM
     */
    Reader(storageDesr_t storageID, storage_size_t size);

    /*
     * get data from SM
     */
    std::string read() const;

    ~Reader();

private:
    res_t openRO();
    resHelper::res_t<descr_t> mapM();

    descr_t _mem{nullptr};
};

}
#endif // READER_H
