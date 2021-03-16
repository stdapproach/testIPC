/*
 * test for load [win]dll
 */
#include "libHandler.h"

int main()
{
    using namespace std;
    cout << "Agent1: Hello World!" << endl;

    struct log {
        log(){cout << "log::ctr\n";}
        ~log(){cout<< "log::dtr\n";}
    };
    log log1;

    auto dlib1 = libHandler::dlib("liblib1.dll");
    cout << "dll.ready=" << boolalpha << (dlib1 ? true : false) << endl;

    try {
        using type1 = int(*)();
        if (dlib1) {
            auto f = libHandler::getFunc<type1>(dlib1.handle(), "foo");
            cout << "res=" << f() << endl;
        } else {
            cout << "issue with dlib\n";
        }
    }  catch (...) {
        cout << "exception with function invoking\n";
    }

    return 0;
}
