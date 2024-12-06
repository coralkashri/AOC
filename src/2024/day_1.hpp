#ifndef ADVENTOFCODE2024_DAY_1_HPP
#define ADVENTOFCODE2024_DAY_1_HPP

#include <iostream>
#include <algorithm>
#include <iterator>
#include <map>
#include <ranges>

#include "../input_source.hpp"
#include "../tools/str_number.hpp"
#include "../tools/back_insert_sort_iterator.hpp"

template <typename T = std::vector<int>>
using insertion_type = std::back_insert_iterator<T>;
//using insertion_type = back_insert_sort_iterator<T>;

int first_part_2024_1() {
    std::vector<int> left, right;
    left.reserve(100);
    right.reserve(100);
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [next_cont = insertion_type<>(left), prev_cont = insertion_type<>(right)] (std::string str) mutable {
        str_number<int> n1(str);
        *next_cont = n1.get_number();
        std::swap(next_cont, prev_cont);
    });

    if constexpr (std::is_same_v<insertion_type<>, std::back_insert_iterator<std::vector<int>>>) {
        std::sort(left.begin(), left.end());
        std::sort(right.begin(), right.end());
    }

    std::cout << std::ranges::fold_left(std::views::zip_transform([](auto n1, auto n2) { return std::abs(n1 - n2); }, left, right), 0ull, std::plus<>{}) << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2024_1() {
    std::unordered_map<int, uint32_t> left, right;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [next_cont = &left, prev_cont = &right] (std::string str) mutable {
        str_number<int> n1(str);
        ++(*next_cont)[n1.get_number()];
        std::swap(next_cont, prev_cont);
    });

    size_t score = 0;
    for (auto [n, times] : left) {
        score += n * times * (right.contains(n) ? right[n] : 0);
    }

    std::cout << score << std::endl;

    return EXIT_SUCCESS;
}

#endif //ADVENTOFCODE2024_DAY_1_HPP
