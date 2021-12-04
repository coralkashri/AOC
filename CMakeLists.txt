cmake_minimum_required(VERSION 3.20)
project(AdventOfCode2021)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCE_FILES src/*)

add_executable(AdventOfCode2021 ${SOURCE_FILES})