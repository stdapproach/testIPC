#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <sstream>

#include "libHandler.h"
#include "gateway.h"
#include "reader.h"
#include "sem.h"

#include "ipcH.h"

#include "processHelper.h"

using namespace std;

namespace {
using PCh = const char*;
static auto pid{processHelper::getpid()};

using fName = gateaway::handlerRegistr::fName;
using Func = gateaway::handlerRegistr::Func;
using TStr = std::string;
using TDict = std::map<TStr, Func>;
using TName_Func = std::pair<fName, Func>;

resHelper::res_t<TName_Func> getFuncFromDLib(libHandler::dlib& dl) {
    auto f1 = libHandler::getFunc<fName>(dl.handle(), "handlerName");
    auto f2 = libHandler::getFunc<Func>(dl.handle(), "handlerFunc");
    if (f1 && f2) {
        return {{f1, f2}};
    }
    return resHelper::err_t{2, "wrong getting function"};
};

std::vector<TStr> toVector(int argc, char *argv[]) {
    std::vector<TStr> res;
    for (auto i = 0; i< argc; ++i) {
        res.emplace_back(argv[i]);
    }
    return res;
}

static const size_t indexSM_Name{1};
static const size_t indexSM_Size{2};
static const size_t semaphoreName{3};
static const size_t startIndexForLibs{4};

resHelper::res_t<TStr> getNonEmptyString(uint index, std::vector<TStr> params) {
    if (index >= params.size()) {
        return resHelper::err_t{1, "index out of bound"};
    }
    auto res = params[index];
    if (res.empty()) {
        return resHelper::err_t{2, "empty string"};
    }
    return {res};
}

resHelper::res_t<TStr> getSM_Name(std::vector<TStr> params) {
    return getNonEmptyString(indexSM_Name, params);
}

resHelper::res_t<uint> getSM_Size(std::vector<TStr> params) {
    int res{0};
    static const int defCodeErr{1};
    static const resHelper::err_t defError{defCodeErr, "wrong conversion char[] to int\n"};
    try {
        std::stringstream ss(params.at(indexSM_Size));
        if( ss >> res ) {
            if (res <= 0) {
                resHelper::err_t{defCodeErr, "size should be positive"};
            }
            return {static_cast<uint>(res)};
        }
        else {
            return defError;
        }
    } catch (...) {
        return defError;
    }
}

resHelper::res_t<bool> addFuncToDict(libHandler::dlib& dl, TDict& dict) {
    static const int defCodeErr{1};
    auto res = getFuncFromDLib(dl);
    if (!res.hasError()) {
        auto ptr = res.val.first;
        if (ptr) {
            auto name = res.val.first();
            dict.emplace(name, res.val.second);
            return {true};
        }
    }
    return resHelper::err_t{defCodeErr, "null function pointer"};
}

resHelper::res_t<std::pair<TStr, TStr>> gettingDataFromSM(PCh name, uint size) {
    try {
        sm::Reader reader(name, size);
        auto buff = reader.read();
        std::stringstream ss(buff);
        TStr serviceName;
        TStr serviceParam;
        ss >> serviceName;
        ss >> serviceParam;
        return {{serviceName, serviceParam}};
    }  catch (...) {
        return resHelper::err_t{1, "wrong read from SM"};
    }
}

}

/*
 * argv[0] - file name
 * argv[1] - sharedMemory\s name
 * argv[2] - sharedMemory\s size
 * argv[3] - semaphor's name
 * argv[4..] - (optional) names for preloaded libs
 */
int main(int argc, char *argv[])
{
    cout << "gateway: argc=" << argc << endl;

    std::vector<libHandler::dlib> vec;
    std::map<TStr, Func> Dict;
    auto vArgv = toVector(argc, argv);

    if (vArgv.size() > 4) {
        resHelper::res_t<TStr> nameSM = getSM_Name(vArgv);
        if(nameSM.hasError()) {
            return resHelper::retErr(nameSM);
        }
        auto idSM{nameSM.val.c_str()};
        cout << "name(SM1)=" << idSM << endl;

        resHelper::res_t<uint> resSizeSM = getSM_Size(vArgv);
        if(resSizeSM.hasError()) {
            return resHelper::retErr(resSizeSM);
        }
        uint sizeSM{resSizeSM.val};
        std::cout << "size(SM1)=" << sizeSM <<endl;

        auto rawSemName = getNonEmptyString(semaphoreName, vArgv);
        if (rawSemName.hasError()) {
            return resHelper::retErr(rawSemName);
        }
        auto semName = rawSemName.val;
        cout << "semName=" << semName << endl;


        cout << "prepared handlers from auto-loaded dLib\n";
        {//first fulfilling of Dict
            copy(vArgv.begin()+startIndexForLibs, vArgv.end(), back_inserter(vec));
            std::for_each(begin(vec), end(vec),
                [&Dict](auto& dl) {
                    addFuncToDict(dl, Dict);
            });
        }

        cout << "getting data from SM\n";
        resHelper::res_t<std::pair<TStr, TStr>> smData = gettingDataFromSM(idSM, sizeSM);
        if (smData.hasError()) {
            return resHelper::retErr(smData);
        }
        TStr serviceName{smData.val.first};
        TStr serviceParam{smData.val.second};

        {//search param among the loaded map
            auto key = serviceName.c_str();
            auto it = Dict.find(key);
            if (it == Dict.end()) {
                //load dLib by name of service
                constexpr auto os{helper::osSelector()};
                auto helper = libHandler::helperT<os>{};
                TStr name = helper.prefixLib+key;
                vec.emplace_back(helper.getFullDLibName(name));
                auto res2 = getFuncFromDLib(vec.back());
                if (!res2.hasError()) {
                    auto descr = res2.val;
                    auto name = descr.first();
                    Dict.emplace(name, descr.second);
                } else {
                    cerr << "wrong getting function from " << vec.back().name() << endl;
                    return 1;
                }
                it = Dict.find(key);
                if (it == Dict.end()) {
                    cerr << "NOT FOUND key=" << key << " after loading dLib by name"<< endl;
                    return 1;
                }
            }
            //invoking
            const auto RESULT{it->second(serviceParam.c_str())};
            cout << "RESULT=" << RESULT << endl;

            //write result
            ipc::Writer writer{idSM, sizeSM, "Gateway"};

            sm::Sem sem{semName.c_str()};
            sem.acquire();
            printf("Gateway: I am done! Release Semaphore\n");
            {
                writer.write("QQQ_AAA_ZZZ");
            }
            if (sem_post(sem.id()) < 0)
                cerr << "Gateway: [sem_post] Failed \n";
            cout << "Gateway: ok releasing sem\n";
        }
    } else {
        cerr << "not enough parameters to invoke\n";
        return 1;
    }

    cout << "gateway: Leaving" << endl;
    return 0;
}
