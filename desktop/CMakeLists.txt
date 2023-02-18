cmake_minimum_required(VERSION 3.1.0)
project(oceannes)


# add sources
file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/src/Emulator.cpp
    ${PROJECT_SOURCE_DIR}/src/VirtualScreen.cpp
)

# 第三方库文件夹
set (THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/3rdparty)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # SFML文件夹
    set (SFML_DIR ${THIRDPARTY_DIR}/mac/SFML-2.5.1)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set (SFML_DIR ${THIRDPARTY_DIR}/windows/SFML-2.5.1) 
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set (SFML_DIR ${THIRDPARTY_DIR}/linux/SFML-2.5.1) 
endif()




find_package(SFML 2.5 COMPONENTS  audio network graphics window system REQUIRED)

if (NOT SFML_FOUND)
    message(FATAL_ERROR "SFML NOT FOUND!")
else()
    message("SFML FOUND!")
endif()


# include directory
include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(oceannes ${SOURCES})

target_link_libraries(oceannes sfml-audio sfml-network sfml-graphics sfml-window sfml-system)