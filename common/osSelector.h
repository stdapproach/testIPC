#ifndef OSSELECTOR_H
#define OSSELECTOR_H

namespace helper {

enum class supportedOS {
    //unknown,
    windows,
    posix
};

constexpr supportedOS osSelector()
{
#ifdef _WIN64
    return supportedOS::windows;//Windows (64-bit)
    #define OSSELECTOR_WIN
#elif _WIN32
    return supportedOS::windows;//Windows (32-bit)
    #define OSSELECTOR_WIN
#elif __APPLE__
    //apple isn't supported
#elif __linux
    return supportedOS::posix;
    #define OSSELECTOR_POSIX
#elif __unix // all unices not caught above
    return supportedOS::posix;
#elif __posix
    return supportedOS::posix;
#else
    return supportedOS::unknown;
#endif
}

}


#endif // OSSELECTOR_H
