cmake_minimum_required(VERSION 3.16)
project(oceannes)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

include_directories(${PROJECT_SOURCE_DIR}/include)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(THIRD_DIR ${PROJECT_SOURCE_DIR}/thirds)

# 使用指定的文件
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(${CMAKE_SYSTEM_NAME})
    set(SFML_DIR ${THIRD_DIR}/SFML/windows-gcc/SFML-2.6.0/lib/cmake/SFML)
    message(${SFML_DIR})
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(SFML_DIR ${THIRD_DIR}/SFML/mac/SFML-2.6.0/Frameworks/SFML.framework/Resources/CMake)
endif()
#  network
find_package(SFML 2.6 COMPONENTS audio  graphics window system REQUIRED)

if(NOT SFML_FOUND)
    message(FATAL_ERROR "SFML NOT FOUND!")
else()
    message("SFML FOUND!")
endif()

aux_source_directory(./src  SRC_LIST )

add_executable(oceannes main.cc ${SRC_LIST})

# sfml-network
target_link_libraries(oceannes sfml-audio sfml-graphics sfml-window sfml-system)