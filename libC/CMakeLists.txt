cmake_minimum_required(VERSION 3.14)

project(libC LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(libC SHARED
  ../common/lib_global.h
  ../common/libCommon.h
  libC.cpp
)

set_target_properties(libC
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin
)

include_directories(../common)
target_compile_definitions(libC PRIVATE LIBC_LIBRARY)
