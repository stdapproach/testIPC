#include <iostream>

#include "processHelper.h"
#include "reader.h"

namespace {
static auto pid{processHelper::getpid()};
}

/*
 * Demo reader SharedMemory from
 */
int main(int /*argc*/, char */*argv*/[])
{
    using namespace std;
    try {
        sm::Reader reader(paramComm::ID, paramComm::size);
        auto buff = reader.read();

        cout <<"QQQ_PID " << pid << ": Read from shared memory:\n----"
        << buff.c_str()
        << "----" << endl;
        return 0;
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
}
