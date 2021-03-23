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
        std::cout << "Dropping semaphore name=" << _name << std::endl;
        if ((_semID = sem_open(_name, 0)) == SEM_FAILED ) {
            return resHelper::err_t{1, "wrong sem_open"};
        }
        std::cout << "sem_post\n";
        sem_post(_semID);
        std::cout << "Semaphore dropped.\n";
        return 0;
    }

    void close() {
        std::cout << "Semaphore close, name=" << _name << std::endl;
        if (sem_close(_semID) != 0){
            std::cerr << "Semaphore: [sem_close] Failed\n";
            return;
        }
        if (sem_unlink(_name) < 0){
            std::cerr << "Semaphore: [sem_unlink] Failed\n";
            return;
        }
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
