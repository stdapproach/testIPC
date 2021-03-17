#include <iostream>
#include <sstream>

#include "processHelper.h"
#include "writer.h"

namespace {
static auto pid{processHelper::getpid()};
}

int main(int /*argc*/, char */*argv*/[])
{
    using namespace std;

    cout << "QQQ_1\n";
    std::ostringstream ss;
    ss << "Hello, World! From PID " << pid;
    std::string data = ss.str();

    try {
        sm::Writer writer(paramComm::ID, paramComm::size);
        cout << "QQQ_memcpy from pid=" << pid << endl;
        writer.write(data);

        // wait for someone to read it
        sleep(10);
    //    while(true) {
    //        sleep(10);
    //    }
    } catch (const char* e) {
        cout << "Exception: " << e << endl;
        throw;
    }
    catch (const std::runtime_error& e) {
        cout << "Runtime error: " << e.what() << endl;
        throw;
    }
    catch (...) {
        cout << "unknown exception\n";
        throw;
    }
    cout << "QQQ_BYE\n";
    return 0;
}
