#ifndef AOC_2021_2
#define AOC_2021_2

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include "../input_source.hpp"

int first_part_2021_2() {
    int horizontal = 0, depth = 0, *relevant_measurement;
    int sign;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [&] (std::string str) {
        if (str == "forward") {
            relevant_measurement = &horizontal;
            sign = 1;
        } else if (str == "down") {
            relevant_measurement = &depth;
            sign = 1;
        } else if (str == "up") {
            relevant_measurement = &depth;
            sign = -1;
        } else {
            *relevant_measurement += sign * std::stoi(str);
        }
    });
    std::cout << depth * horizontal << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2021_2() {
    int horizontal = 0, depth = 0, aim = 0, *relevant_measurement;
    int sign;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [&] (std::string str) {
        if (str == "forward") {
            relevant_measurement = &horizontal;
            sign = 1;
        } else if (str == "down") {
            relevant_measurement = &aim;
            sign = 1;
        } else if (str == "up") {
            relevant_measurement = &aim;
            sign = -1;
        } else {
            auto current_number = std::stoi(str);
            *relevant_measurement += sign * current_number;
            if (relevant_measurement == &horizontal) {
                depth += aim * current_number;
            }
        }
    });
    std::cout << depth * horizontal << std::endl;

    return EXIT_SUCCESS;
}

#endif