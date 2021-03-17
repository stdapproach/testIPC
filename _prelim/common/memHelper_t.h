#ifndef MEMHELPER_T_H
#define MEMHELPER_T_H

#include "resHelper.h"
#include "paramComm.h"

namespace memHelper {
using storageDesr_t = paramComm::storageDesr_t;
using storage_size_t = paramComm::storage_size_t;
using descr_t = void*;
using res_t = resHelper::res_t<int>;
}

#endif // MEMHELPER_T_H
