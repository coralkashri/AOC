cmake_minimum_required(VERSION 3.20)
project(AdventOfCode2021)

set(CMAKE_CXX_STANDARD 23)

add_executable(AdventOfCode2021 main.cpp 2021/day_1.hpp 2021/day_2.hpp 2021/day_3.hpp 2021/day_4.hpp 2020/day_1.hpp input_source.hpp main_definitions_helper.hpp)
