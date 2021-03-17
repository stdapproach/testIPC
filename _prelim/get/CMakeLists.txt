cmake_minimum_required(VERSION 3.5)

project(get LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(../common)

add_executable(get main.cpp
    ../common/readHelper.h
    ../common/reader.h
)
target_link_libraries(get -lrt)
