#include <iostream>
#include <string>
#include <functional>

#include "processHelper.h"
#include "libHandler.h"
#include "osSelector.h"
#include "ipcH.h"
#include "sem.h"

#include "sm_public.h"

namespace {
using strT = std::string;
using PCh = const char*;

constexpr auto os{helper::osSelector()};

struct smDescr {
    PCh name;
    const sm::storage_size_t size;
    smDescr() = delete;
};

struct testEnv {
    PCh serviceName;
    PCh argument;
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

std::string mkRunningString(strT rawName, smDescr descr, PCh semName, std::vector<strT> preLoadedLib) {
    auto helper = libHandler::helperT<os>{};
    auto makeParamsRunningFor = [&descr, &helper, &preLoadedLib, semName]() {
        using namespace std::placeholders;
        std::vector<strT> res {
                    descr.name,
                    std::to_string(descr.size),
                    semName
                };
        auto adjNames = transform_container(
                    preLoadedLib,
                    std::bind(&libHandler::helperT<os>::getFullDLibName, &helper, _1));
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
    static const char* appPrefix{"Tester: "};
    using namespace std;

    cout << appPrefix << "Starting" << endl;
    const smDescr sm1{"/smIn", 128};
    const strT rawName{"gateway"};
    const std::vector<strT> preLoadedLib{"liblib1", "liblib2"};
    testEnv tEnv{"Type_C", "123456"};
    auto helper = libHandler::helperT<os>{};
    PCh semName{"/sem5"};
    try {
        sm::Sem sem(semName);
        sem.acquire();

        const bool unlinkRequired{false};
        ipc::Writer writer{sm1.name, sm1.size, appPrefix, unlinkRequired};
        writer.write(mkData(tEnv));

        auto paramForRun = mkRunningString(rawName, sm1, semName, preLoadedLib);
        system(paramForRun.c_str());
        {
            if (sem_wait(sem.id()) < 0)
                perror("Tester: sem_wait");
            if (sem_close(sem.id()) < 0)
                perror("Tester: sem_close");
        }

        auto returnedResult = writer.read();
        cout << appPrefix <<"returnedResult=" << returnedResult.c_str() << endl;
    }  catch (...) {
        cout << appPrefix <<"unknown exception raised" << endl;
        throw ;
    }
    return 0;
}
