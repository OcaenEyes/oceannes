cmake_minimum_required(VERSION 3.16)
project(oceannes)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(SRC_LIST main.cc ./src)

add_executable(oceannes ${SRC_LIST})