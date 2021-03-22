cmake_minimum_required(VERSION 3.5)

project(smLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../build)

include_directories(../common
)

set(SOURCE_FILES sm.cpp
    writeHelper.cpp
    writer.cpp
    readHelper.cpp
    reader.cpp
)
set(HEADER_FILES sm_public.h
    memHelper.h
    memHelper_t.h
    memHelper.h
    ../common/resHelper.h
    writeHelper.h
    writer.h
    readHelper.h
    reader.h
    sem.h
)

add_library(smLib-static STATIC ${HEADER_FILES} ${SOURCE_FILES})
