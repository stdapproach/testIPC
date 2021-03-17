#ifndef PROCESS_H
#define PROCESS_H

#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

/*
 * to handle processes
 */
namespace process {

using strT = std::string;

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
