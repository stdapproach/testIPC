cmake_minimum_required(VERSION 3.5)

project(set LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(../common)

add_executable(set main.cpp
    ../common/paramComm.h
    ../common/resHelper.h
    ../common/processHelper.h
    ../common/writeHelper.h
    ../common/memHelper.h
    ../common/memHelper_t.h
    ../common/writer.h
)
target_link_libraries(set -lrt)
