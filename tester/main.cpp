#include <iostream>
#include <string>

#include "processH.h"
#include "osSelector.h"

namespace {
using strT = std::string;
template<helper::supportedOS os>
struct helperT;

template<>
struct helperT<helper::supportedOS::posix> {
    const strT prefix{"./"};
    const strT postfixDLib{".*so"};
    const strT postfixExec{""};
    strT getFullExecName(strT rawName) {
        return prefix + rawName + postfixExec;
    }
    strT getFullDLibName(strT rawName) {
        return prefix + rawName + postfixDLib;
    }
};
constexpr auto os{helper::osSelector()};

static const strT rawName{"gateway"};
}

int main()
{
    using namespace std;
    cout << "Tester: Hello World!" << endl;
    auto helper = helperT<os>{};
    try {
        //run with param - list of required dLib
        const std::vector<strT> desiredDLibs {
            helper.getFullDLibName("liblib1")
            ,helper.getFullDLibName("liblib2")
        };
        auto execName = helper.getFullExecName(rawName);
        process::descr descr{execName, desiredDLibs};
        system(descr.toStr().c_str());
    }  catch (...) {
        cout << "Tester: unknown exception raised" << endl;
        throw ;
    }
    cout << "Tester: Leaving" << endl;
    return 0;
}
