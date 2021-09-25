cmake_minimum_required(VERSION 3.21)
project(android_battery_check VERSION 1.0.2)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(SOURCES
    src/main.cpp
    src/cmd.cpp
    src/parser.cpp
)

add_executable(android_battery_check ${SOURCES})
