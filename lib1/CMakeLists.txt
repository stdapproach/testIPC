cmake_minimum_required(VERSION 3.14)

project(lib1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(lib1 SHARED
  ../common/lib_global.h
  ../common/libCommon.h
  ../common/strHelper.h
  lib1.cpp
)

set_target_properties(lib1
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin
)

include_directories(../common)
target_compile_definitions(lib1 PRIVATE LIB1_LIBRARY)
