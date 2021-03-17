#ifndef RESHELPER_H
#define RESHELPER_H

#include <string>

namespace resHelper {
static const int no_err{0};
struct err_t {
    int code;
    std::string descr;
    err_t():code{no_err}, descr{""}{};
    err_t(int c, std::string d):code{c}, descr{d}{};
    bool has()const{return (code != no_err);}
};
template<typename T>
struct res_t {
    using val_t = T;
    err_t error;
    T val;
    res_t(T t):hasResult{true},val{t}{}
    res_t(err_t err):hasResult{false},error{err}{}
    bool hasError()const{return error.has();}
private:
    bool hasResult{false};
};
}

#endif // RESHELPER_H
