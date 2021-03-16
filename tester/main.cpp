#include <iostream>

#include "processH.h"

int main()
{
    using namespace std;
    cout << "Tester: Hello World!" << endl;
    {//run with param - list of required dll
        process::descr descr{"gateway.exe", {"liblib1.dll","liblib2.dll"}};
        system(descr.toStr().c_str());
    }
    cout << "Tester: Leaving" << endl;
    return 0;
}
