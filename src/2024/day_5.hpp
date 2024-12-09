#ifndef ADVENTOFCODE2024_DAY_5_HPP
#define ADVENTOFCODE2024_DAY_5_HPP

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

int first_part_2024_5() {
    std::map<int, std::set<int>> forbidden_numbers_after_number;

    std::cout << std::transform_reduce(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), 0ull, std::plus{}, [&forbidden_numbers_after_number, printing_phase = false](std::string str) mutable {
        std::string_view str_view = str;
        if (!printing_phase) {
            size_t sep_index = str_view.find('|');

            if (sep_index != std::string::npos) {
                str_number<int> first = str_view.substr(0, sep_index);
                str_number<int> second = str_view.substr(sep_index + 1);
                forbidden_numbers_after_number[second.get_number()].insert(first.get_number());
                return 0;
            } else {
                printing_phase = true;
                if (str_view.empty()) return 0;
            }
        }

        auto numbers = str_numbers<int, std::vector, ','>(str_view).get();
        auto forbidden_numbers = forbidden_numbers_after_number[numbers.front()];

        bool is_first = true;
        for (int n : numbers) {
            if (is_first) {
                is_first = false;
                continue;
            }

            if (forbidden_numbers.contains(n)) return 0;

            const auto& more_forbidden_numbers = forbidden_numbers_after_number[n];
            forbidden_numbers.insert(more_forbidden_numbers.begin(), more_forbidden_numbers.end());
        }

        return numbers[numbers.size() / 2];
    }) << "\n";

    return EXIT_SUCCESS;
}

int fix_incorrect_ordering(std::span<int> numbers, const std::map<int, std::set<int>>& forbidden_numbers_after_number) {
    for (size_t i = 1; i < numbers.size(); ++i) {
        for (size_t j = 0; j < i; ++j) {
            if (forbidden_numbers_after_number.at(numbers[j]).contains(numbers[i])) {
                std::swap(numbers[i], numbers[j]);
                i = 0;
                break;
            }
        }
    }

    return numbers[numbers.size() / 2];
}

int second_part_2024_5() {
    std::map<int, std::set<int>> forbidden_numbers_after_number;

    std::cout << std::transform_reduce(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), 0ull, std::plus{}, [&forbidden_numbers_after_number, printing_phase = false](std::string str) mutable {
        std::string_view str_view = str;
        if (!printing_phase) {
            size_t sep_index = str_view.find('|');

            if (sep_index != std::string::npos) {
                str_number<int> first = str_view.substr(0, sep_index);
                str_number<int> second = str_view.substr(sep_index + 1);
                forbidden_numbers_after_number[second.get_number()].insert(first.get_number());
                return 0;
            } else {
                printing_phase = true;
                if (str_view.empty()) return 0;
            }
        }

        auto numbers = str_numbers<int, std::vector, ','>(str_view).get();
        auto forbidden_numbers = forbidden_numbers_after_number[numbers.front()];

        bool is_first = true;
        for (int n : numbers) {
            if (is_first) {
                is_first = false;
                continue;
            }

            if (forbidden_numbers.contains(n)) {
                return fix_incorrect_ordering(numbers, forbidden_numbers_after_number);
            }

            const auto& more_forbidden_numbers = forbidden_numbers_after_number[n];
            forbidden_numbers.insert(more_forbidden_numbers.begin(), more_forbidden_numbers.end());
        }

        return 0;
    }) << "\n";

    return EXIT_SUCCESS;
}

#endif
