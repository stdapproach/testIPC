#ifndef SEM_H
#define SEM_H

#include <iostream>

#include <semaphore.h>
#include <fcntl.h>

#include "resHelper.h"
#include "processHelper.h"

namespace sm {
using PCh = const char*;

struct Sem {
    Sem() = delete;
    Sem(const Sem&) = delete;
    Sem& operator=(Sem&) = delete;
    Sem(PCh name) : _name{name} {}

    void acquire() {
        try {
            _semID = sem_open(_name, O_CREAT, 0600, 0);
            auto pid = processHelper::getpid();
            if (_semID == SEM_FAILED) {
                std::cerr << "PID: " << pid << " [sem_open], name=" << _name << " failed\n";
                return;
            }
            _isOpened = true;
        } catch(const std::exception& e) {
            std::cout << "Sem, exception while opening: " << e.what() << std::endl;
            throw;
        }
        catch (...) {
            std::cerr << "Unknown exception while sem_open\n";
            throw;
        }
    }

    resHelper::res_t<int> release() {
        if ((_semID = sem_open(_name, 0)) == SEM_FAILED ) {
            return resHelper::err_t{1, "wrong sem_open"};
        }
        try {
            auto res = sem_post(_semID);
            return {res};
        } catch (...) {
            std::cerr << "Exception while releasing semaphore name="
                      << _name << std::endl;
            throw;
        }
    }

    int wait()const {
        int res = sem_wait(_semID);
        if (res < 0) {
            std::cerr << "Semaphore: [sem_wait] Failed\n";
            return res;
        }
        return 0;
    }

    int close() {
        int res1 = sem_close(_semID);
        if (res1 < 0){
            std::cerr << "Semaphore: [sem_close] Failed\n";
            return res1;
        }
        int res2 = sem_unlink(_name);
        if (res2 < 0){
            std::cerr << "Semaphore: [sem_unlink] Failed\n";
            return res2;
        }
        return 0;
    }

    ~Sem() {
    }

    sem_t* id(){return _semID;}

private:
    PCh _name;
    sem_t* _semID;
    bool _isOpened{false};
};

}

#endif // SEM_H
