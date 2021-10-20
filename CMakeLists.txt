cmake_minimum_required(VERSION 3.21)
project(android_battery_check VERSION 1.1.0)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Write current version to header file
configure_file(src/version.h.in version.h)

# Set compiler flags
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(SOURCE_FILES
    src/main.cpp
    src/cmd.cpp
    src/parser.cpp
)

add_executable(android_battery_check ${SOURCE_FILES})

target_include_directories(android_battery_check PRIVATE "${PROJECT_BINARY_DIR}")
