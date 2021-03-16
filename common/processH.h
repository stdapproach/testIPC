#ifndef PROCESS_H
#define PROCESS_H

#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

#include "types.h"

/*
 * to handle processes
 */
namespace process {

struct descr {
    types::Str processFullPath;
    std::vector<types::Str> params;

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
