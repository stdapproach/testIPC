/*
 * test for load [win]dll
 */
#include "libHandler.h"

int main()
{
    using namespace std;
    cout << "2Hello World!" << endl;
    //libHandler::loadLib("liblib1.dll");

    libHandler::dlib("liblib1.dll");
    cout << "QQQ\n";

    return 0;
}
