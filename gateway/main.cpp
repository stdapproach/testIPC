#include <vector>
#include <algorithm>
#include <functional>

#include "libHandler.h"
#include "gateway.h"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "gateway: argc=" << argc << endl;

    std::vector<libHandler::dlib> vec;
    using fName = gateaway::handlerRegistr::fName;
    using Func = gateaway::handlerRegistr::Func;
    std::vector<std::pair<fName, Func>> vH;

    if (argc>1) {
        for(auto i=1; i<argc; ++i) {
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
        for(auto it: vH) {
            const auto mockParam{"QQQ"};
            cout << "name=" << it.first() << ", res(" << mockParam <<")="
                 << it.second(mockParam) << endl;
        }
    }

    cout << "gateway: Leaving" << endl;
    return 0;
}
