#ifndef STORAGEDESCR_H
#define STORAGEDESCR_H

#include <stddef.h>

namespace paramComm{
    using storageDesr_t = const char*;
    using storage_size_t = size_t;
    static storageDesr_t ID{"/SHM_TEST"};
    static const storage_size_t size{32};
}

#endif // STORAGEDESCR_H
