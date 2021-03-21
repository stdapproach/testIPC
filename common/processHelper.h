#ifndef PROCESS_H
#define PROCESS_H

#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

#include <unistd.h>

/*
 * to handle processes
 */
namespace processHelper {
using _t = pid_t;
using strT = std::string;

_t getpid(){return ::getpid();}

struct descr {
    strT processFullPath;
    std::vector<strT> params;

    /*
     * make the string to run process with add. params
     */
    std::string toStr() const {
        std::stringstream stream;
        stream << processFullPath;
        stream << " " ;
        for_each (std::begin(params), std::end(params),
            [&stream](auto it){
                stream << it << " ";
        });
        return stream.str();
    }
};

}

#endif // PROCESS_H
