#ifndef ADVENTOFCODE2024_DAY_7_HPP
#define ADVENTOFCODE2024_DAY_7_HPP

#include <iostream>
#include <algorithm>
#include <iterator>
#include <map>
#include <ranges>
#include <numeric>
#include <regex>

#include "../input_source.hpp"
#include "../tools/str_number.hpp"
#include "../tools/word_delimited_by.h"
#include "../tools/matrix.hpp"
#include "../tools/point.hpp"
#include <set>
#include "../tools/range.hpp"

template <typename FuncT>
bool try_combinations(std::span<size_t> numbers, size_t res, FuncT&& func, size_t current_res = 0) {
    if (numbers.empty()) return current_res == res;
    if (try_combinations(numbers.subspan(1), res, std::plus{}, func(numbers.front(), current_res))) return true;
    return try_combinations(numbers.subspan(1), res, std::multiplies{}, func(numbers.front(), current_res));
}

int first_part_2024_7() {
    std::cout << std::transform_reduce(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), 0ull, std::plus{}, [](std::string str){
        std::string_view strv = str;
        auto res_stop_position = strv.find(':');
        auto res = str_number<size_t>(strv.substr(0, res_stop_position)).get_number();
        auto numbers = str_numbers<size_t, std::vector, ' '>(strv.substr(res_stop_position + 2)).get();


        return try_combinations(numbers, res, std::plus{}) ? res : 0;
    }) << "\n";

    return EXIT_SUCCESS;
}

template <typename T>
struct concetration {
    T operator()(const T& x, const T& y) const {
        auto z = y;
        size_t y_size = 1;
        while (z != 0) {
            z /= 10;
            y_size *= 10;
        }

        return x * y_size + y;
    }
};

template <typename FuncT>
bool try_combinations2(std::span<size_t> numbers, size_t res, FuncT&& func, size_t current_res = 0) {
    if (numbers.empty()) return current_res == res;
    if (try_combinations2(numbers.subspan(1), res, std::plus{}, func(current_res, numbers.front()))) return true;
    if (try_combinations2(numbers.subspan(1), res, std::multiplies{}, func(current_res, numbers.front()))) return true;
    return try_combinations2(numbers.subspan(1), res, concetration<size_t>{}, func(current_res, numbers.front()));
}

int second_part_2024_7() {
    std::cout << std::transform_reduce(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), 0ull, std::plus{}, [](std::string str){
        std::string_view strv = str;
        auto res_stop_position = strv.find(':');
        auto res = str_number<size_t>(strv.substr(0, res_stop_position)).get_number();
        auto numbers = str_numbers<size_t, std::vector, ' '>(strv.substr(res_stop_position + 2)).get();
        return try_combinations2(numbers, res, std::plus{}) ? res : 0;
    }) << "\n";

    return EXIT_SUCCESS;
}

#endif
