#ifndef PROCESSHELPER_H
#define PROCESSHELPER_H

#include <unistd.h>

namespace processHelper {
using _t = pid_t;
_t getpid(){return ::getpid();}
}

#endif // PROCESSHELPER_H
