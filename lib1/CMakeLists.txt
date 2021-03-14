cmake_minimum_required(VERSION 3.14)

project(lib1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(lib1 SHARED
  lib1_global.h
  lib1.cpp
  lib1.h
)

set_target_properties(lib1
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../build
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin
)


target_compile_definitions(lib1 PRIVATE LIB1_LIBRARY)
