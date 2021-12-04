#ifndef AOC_2021_1
#define AOC_2021_1

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include "../input_source.hpp"

int first_part_2021_1() {
    size_t previous_number = 0xFFFFFFFFFFFFFFFF;
    std::cout << std::count_if(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [&] (std::string str) {
        auto current_number = std::stoi(str);
        bool res = current_number > previous_number;
        previous_number = current_number;
        return res;
    }) << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2021_1() {
    std::vector<size_t> previous_measurements;
    size_t sum;
    std::cout << std::count_if(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [&] (std::string str) {
        bool res = false;
        size_t current_num = std::stoll(str);
        if (previous_measurements.size() < 3) {
            previous_measurements.emplace_back(current_num);
            sum += current_num;
        } else {
            res = sum + current_num - previous_measurements.front() > sum;
            std::rotate(previous_measurements.begin(), previous_measurements.begin() + 1, previous_measurements.end());
            previous_measurements.back() = current_num;
        }
        return res;
    }) << std::endl;

    return EXIT_SUCCESS;
}
#endif