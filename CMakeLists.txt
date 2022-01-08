cmake_minimum_required(VERSION 3.18)
project(AdventOfCode2021)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++20 -ltbb -static-libstdc++")

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCE_FILES src/*)

add_executable(AdventOfCode2021 ${SOURCE_FILES})