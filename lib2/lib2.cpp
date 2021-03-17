#include "libCommon.h"
#include <string>
#include <iostream>

/*
 * func for testing dll/so interface
 */
int foo()
{
    return 43;
}

/*
 * provide name for Handler
 */
const char* handlerName()
{
    static const char* name{"Type_B"};
    return name;
}

/*
 * provide Handler's function
 */
int handlerFunc(const char* str)
{
    int mockResult = std::string(str).size()*2;
    return mockResult;
}
