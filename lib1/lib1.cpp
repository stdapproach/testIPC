#include "libCommon.h"
#include <string>
#include <iostream>

/*
 * func for testing dll/so interface
 */
int foo()
{
    return 42;
}

/*
 * provide name for Handler
 */
types::Str handlerName()
{
    static const types::Str name{"Type_A"};
    return name;
}

/*
 * provide Handler's function
 */
int handlerFunc(types::Str str)
{
    int mockResult = std::string(str).size();
    return mockResult;
}
