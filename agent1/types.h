#ifndef TYPES_H
#define TYPES_H

#include <exception>
#include <utility>

namespace types
{
typedef const char* Str;

template<typename T>
class Maybe {//??????
public:
    struct bad_access : std::exception {
        virtual const char* what() const noexcept {
            return "Attempted to access a value of a Maybe wrapper that doesn't exist.";
        }
    };

    Maybe() noexcept : isSettled{false}{}
    Maybe(T t) noexcept : val{t}, isSettled{true}{}
    Maybe(const Maybe<T>& other) noexcept : val{other.val}, isSettled{true} {}

    Maybe& operator=(Maybe<T> other) {
        swap(*this, other);
        return *this;
    }

    friend void swap(Maybe<T>& a, Maybe<T>& b) {
        using std::swap;
        swap(a.val, b.val);
    }

    Maybe(Maybe<T>&& o) noexcept :
        val{std::move(o.val)},
        isSettled{std::exchange(o.isSettled, false)}
    {}

    inline bool empty() const {
        return !isSettled;
    }

    inline bool hasValue() const {
        return !empty();
    }

    inline explicit operator bool() const {
        return hasValue();
    }

    T value() {
        if (empty())
            throw bad_access();
        else
            return val;
    }

    T valueOr(T defaultVal) {
        if (empty())
            return defaultVal;
        return val;
    }

    void clear() {
        isSettled = false;
    }

private:
    T val;
    bool isSettled;
};
}

#endif // TYPES_H
