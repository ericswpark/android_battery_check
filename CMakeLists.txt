cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)


project(android_battery_check)

set(SOURCES
    src/main.cpp
    src/cmd.cpp
    src/parser.cpp
)

add_executable(android_battery_check ${SOURCES})
