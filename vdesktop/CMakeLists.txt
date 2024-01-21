cmake_minimum_required(VERSION 3.16)
project(oceannes)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(THIRD_DIR ${PROJECT_SOURCE_DIR}/thirds)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(SFML_DIR ${THIRD_DIR}/SFML/windows-vs/SFML-2.6.0)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(SFML_DIR ${THIRD_DIR}/SFML/mac/SFML-2.6.0)
endif()

#  network
find_package(SFML 2.6 COMPONENTS audio  graphics window system REQUIRED)

if(NOT SFML_FOUND)
    message(FATAL_ERROR "SFML NOT FOUND!")
else()
    message("SFML FOUND!")
endif()


set(SRC_LIST main.cc ./src)

add_executable(oceannes ${SRC_LIST})

# sfml-network
target_link_libraries(oceannes sfml-audio sfml-graphics sfml-window sfml-system)