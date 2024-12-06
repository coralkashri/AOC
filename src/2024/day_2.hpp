#ifndef ADVENTOFCODE2024_DAY_2_HPP
#define ADVENTOFCODE2024_DAY_2_HPP

#include <iostream>
#include <algorithm>
#include <iterator>
#include <map>
#include <ranges>

#include "../input_source.hpp"
#include "../tools/str_number.hpp"
#include "../tools/word_delimited_by.h"

bool is_safe_test(const std::vector<int>& numbers, size_t ignored_index) {
    uint8_t prev_sign;
    int prev_n;

    if (ignored_index == 0) {
        prev_n = 2 * numbers[1] - numbers[2];
        prev_sign = (numbers[1] - prev_n) >> (sizeof(int) * 8 - 1) & 1;
    } else if (ignored_index == 1) {
        prev_n = 2 * numbers[0] - numbers[2];
        prev_sign = (numbers[0] - prev_n) >> (sizeof(int) * 8 - 1) & 1;
    } else {
        prev_n = 2 * numbers[0] - numbers[1];
        prev_sign = (numbers[0] - prev_n) >> (sizeof(int) * 8 - 1) & 1;
    }

    for (auto &n : numbers) {
        if (&n == numbers.data() + ignored_index) continue;

        auto diff = n - prev_n;
        auto current_sign = diff >> (sizeof(int) * 8 - 1) & 1;
        auto abs_diff = std::abs(diff);
        if (abs_diff > 3 || abs_diff == 0 || prev_sign != current_sign) return false;

        prev_n = n;
    }

    return true;
}

int first_part_2024_2() {
    std::cout << std::count_if(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (auto line) {
        auto numbers = str_numbers<int, std::vector, ' '>(line).get();
        return is_safe_test(numbers, numbers.size());
    }) << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2024_2() {
     std::cout << std::count_if(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (auto line) {
        auto numbers = str_numbers<int, std::vector, ' '>(line).get();

        bool safe = false;
        for (size_t i = 0; !safe && i <= numbers.size(); ++i) {
            safe = is_safe_test(numbers, i);
        }

        return safe;
    }) << std::endl;

    return EXIT_SUCCESS;
}

#endif
