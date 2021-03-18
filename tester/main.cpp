#include <iostream>
#include <string>

#include "processH.h"
#include "osSelector.h"
#include "ipcH.h"

#include "processHelper.h"

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
static const char* sm1_name{"/sm1"};
static const int sm1_size{128};

//static auto pid{processHelper::getpid()};
}

int main()
{
    using namespace std;
    cout << "Tester: Hello World!" << endl;
    auto helper = helperT<os>{};
    try {
        ipc::Writer writer{sm1_name, sm1_size};
        std::ostringstream ss;
        const char* serviceName{"Type_A"};
        const char* argument{"123456"};
        ss << serviceName << " " << argument << " ";
        std::string data = ss.str();
        writer.write(data);

        //run with param - list of required dLib
        const std::vector<strT> params {
            sm1_name, // name of SM
            std::to_string(sm1_size),
            helper.getFullDLibName("liblib1")
            ,helper.getFullDLibName("liblib2")
        };
        auto execName = helper.getFullExecName(rawName);
        process::descr descr{execName, params};
        system(descr.toStr().c_str());
        sleep(10);
    }  catch (...) {
        cout << "Tester: unknown exception raised" << endl;
        throw ;
    }
    cout << "Tester: Leaving" << endl;
    return 0;
}
