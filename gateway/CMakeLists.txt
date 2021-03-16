cmake_minimum_required(VERSION 3.5)

project(gateway LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/../bin)

include_directories(../common)

add_executable(gateway main.cpp
    gateway.h
    ../common/libHandler.h
    )
