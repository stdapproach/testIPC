cmake_minimum_required(VERSION 3.5)

project(gateway LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/../bin)

include_directories(../common
    ../smLib
)

add_executable(gateway main.cpp
    gateway.h
    ../common/processHelper.h
    ../common/libHandler.h
    ../common/libHandlerCommon.h
    ../common/libHandler_Win.h
    ../common/libHandler_Nix.h
    ../common/resHelper.h
    )

find_library(LIB_SM smLib-static ../build)

target_link_libraries(gateway
    ${LIB_SM}
    -ldl
    -lrt
)
