cmake_minimum_required(VERSION 3.5)

project(tester LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/../bin)

include_directories(../common
    ../smLib
)

add_executable(tester main.cpp
    ../common/processHelper.h
    ../common/ipcH.h
    ../common/envHelper.h
    ../smLib/sem.h
)

find_library(LIB_SM smLib-static ../build)

target_link_libraries(tester
    ${LIB_SM}
    -ldl
    -lrt
    -lpthread
    )
