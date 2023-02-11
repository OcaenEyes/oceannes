cmake_minimum_required(VERSION 3.1.0)
project(oceannes)


# add sources
file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# include directory
include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(oceannes ${SOURCES})