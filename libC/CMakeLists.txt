cmake_minimum_required(VERSION 3.14)

project(Type_C LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Type_C SHARED
  ../common/lib_global.h
  ../common/libCommon.h
  libC.cpp
)

set_target_properties(Type_C
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin
)

include_directories(../common)
target_compile_definitions(Type_C PRIVATE LIBC_LIBRARY)
