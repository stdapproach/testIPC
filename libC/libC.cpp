#include "libCommon.h"
#include "strHelper.h"
#include <string>
#include <iostream>
#include <sstream>


/*
 * provide name for Handler
 */
const char* handlerName()
{
    static const char* name{"Type_C"};
    return name;
}

/*
 * provide Handler's function
 */
int handlerFunc(const char* str, char** buffer) {
    using namespace std;
    ostringstream ss;
    ss << "serviceName_" << handlerName() << "_arg_" << str;
    std::string data = ss.str();
    auto res = helper::toBuff(data, buffer);
    return res;
}
