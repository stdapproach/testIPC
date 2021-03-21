#include <iostream>
#include <string>
#include <functional>

#include "processHelper.h"
#include "osSelector.h"
#include "ipcH.h"

#include "sm_public.h"


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

struct smDescr {
    const char* name;
    const sm::storage_size_t size;
    smDescr() = delete;
};

struct testEnv {
    const char* serviceName;
    const char* argument;
};

std::string mkData(testEnv env) {
    std::ostringstream ss;
    ss << env.serviceName << " " << env.argument << " ";
    std::string data = ss.str();
    return data;
}

template <typename Container, typename Functor>
Container transform_container(const Container& c, Functor &&f)
{
    Container ret;
    std::transform(std::begin(c), std::end(c), std::inserter(ret, std::end(ret)), f);
    return ret;
}

std::string mkRunningString(strT rawName, smDescr descr, std::vector<strT> preLoadedLib) {
    auto helper = helperT<os>{};
    auto makeParamsRunningFor = [&descr, &helper, &preLoadedLib]() {
        using namespace std::placeholders;
        std::vector<strT> res {
                    descr.name,
                    std::to_string(descr.size)
                };
        auto adjNames = transform_container(
                    preLoadedLib,
                    std::bind(&helperT<os>::getFullDLibName, &helper, _1));
        std::copy(adjNames.begin(), adjNames.end(),
                      std::back_inserter(res));

        return res;
    };
    processHelper::descr procDescr{helper.getFullExecName(rawName), makeParamsRunningFor()};
    return procDescr.toStr();
}
}

int main()
{
    using namespace std;

    cout << "Tester: Starting" << endl;
    const smDescr sm1{"/sm1", 128};
    const strT rawName{"gateway"};
    const std::vector<strT> preLoadedLib{"liblib1", "liblib2"};
    testEnv tEnv{"Type_B", "123456"};
    auto helper = helperT<os>{};
    try {
        ipc::Writer writer{sm1.name, sm1.size};
        writer.write(mkData(tEnv));

        auto paramForRun = mkRunningString(rawName, sm1, preLoadedLib);
        //cout << "QQQ_str=" << paramForRun.c_str() << endl;
        system(paramForRun.c_str());
        sleep(10);
    }  catch (...) {
        cout << "Tester: unknown exception raised" << endl;
        throw ;
    }
    cout << "Tester: Leaving" << endl;
    return 0;
}
