#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <sstream>

#include "libHandler.h"
#include "gateway.h"
#include "reader.h"

#include "processHelper.h"

using namespace std;

namespace {
static auto pid{processHelper::getpid()};
}
/*
 * argv[0] - file name
 * argv[1] - (optional) sharedMemory\s name
 * argv[2] - (optional) sharedMemory\s size
 * argv[3..] - (optional) names for preloaded libs
 */
int main(int argc, char *argv[])
{
    cout << "2gateway: argc=" << argc << endl;

    std::vector<libHandler::dlib> vec;
    using fName = gateaway::handlerRegistr::fName;
    using Func = gateaway::handlerRegistr::Func;
    std::vector<std::pair<fName, Func>> vH;
    std::map<std::string, Func> vMap;

    if (argc>3) {
        cout << "name(SM1)=" << argv[1] << endl;
        auto ID1{argv[1]};
        uint size1{0};
        {
            try {
                std::stringstream ss(argv[2]);
                if( ss >> size1 ) {
                    std::cout << "size(SM1)=" << size1 <<endl;
                }
                else {
                    std::cout << "error";
                    return -1;
                }
            } catch (...) {
                cout << "wrong conversion char[] to int\n";
                return -1;
            }
        }
        {
            for(auto i=3; i<argc; ++i) {
                vec.emplace_back(argv[i]);
            }
            std::for_each(begin(vec), end(vec),
                [&vH](auto& dl) {
                    auto f1 = libHandler::getFunc<fName>(dl.handle(), "handlerName");
                    auto f2 = libHandler::getFunc<Func>(dl.handle(), "handlerFunc");
                    if (f1 && f2) {
                        vH.emplace_back(f1, f2);
                    } else {
                        cerr << "wrong getting function from " << dl.name() << endl;
                    }
            });

            //convert vector to map
            for (auto it: vH) {
                auto name = it.first();
                vMap.emplace(name, it.second);
            }
        }
        cout << "repared handlers from auto-loaded dLib\n";

        std::string serviceName;
        std::string serviceParam;
        try {//parsing SM1
            sm::Reader reader(ID1, size1);
            auto buff = reader.read();
            cout <<"QQQ_PID " << pid << ": Read from shared memory:\n----"
            << buff.c_str()
            << "----" << endl;

            std::stringstream ss(buff);
            ss >> serviceName;
            ss >> serviceParam;
            cout << "parsing SM: name=" << serviceName << ", param=" << serviceParam << endl;
        }  catch (...) {
            cout << "wrong read from SM1\n";
            return -1;
        }

        {//search param among the loaded map
            auto key = serviceName.c_str();
            auto it = vMap.find(key);
            if (it != vMap.end()) {
                cout << "FOUND " << it->first << endl;
                const auto RESULT{it->second(serviceParam.c_str())};
                cout << "QQQ_RESULT=" << RESULT << endl;
            } else {
                cout << "NOT FOUND key=" << key << "[["<< endl;
            }
        }
    } else {
        cerr << "not enough parameters to invoke\n";
    }

    cout << "gateway: Leaving" << endl;
    return 0;
}
