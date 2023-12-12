#ifndef AOC_2023_12
#define AOC_2023_12

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include <array>
#include <set>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

size_t count_possibilities(std::string_view str, size_t current_number, size_t current_number_index,
                           const std::vector<size_t> &required_numbers, std::string str_completed,
                           char forced_char = ' ') {
    char current_char = str.empty() ? ' ' : forced_char == ' ' ? str[0] : forced_char;
    size_t str_size = str.size();
    str_completed += (current_char == '?' ? "" : std::string() + current_char);

    if (current_number_index == required_numbers.size()) {
        for (char c: str) {
            if (c == '#') return 0;
        }
        return 1;
    }

    if (current_number == required_numbers[current_number_index]) {
        if (str.empty()) {
            return current_number_index + 1 == required_numbers.size();
        }

        if (current_char == '#') {
            return 0;
        } else {
            return count_possibilities(str, 0, current_number_index + 1, required_numbers, str_completed, '.');
        }
    }

    if (str.empty()) {
        return 0;
    }

    switch (current_char) {
        case '.':
            if (current_number > 0 && current_number < required_numbers[current_number_index]) return 0;
            return count_possibilities(str.substr(1), current_number, current_number_index, required_numbers,
                                       str_completed);
        case '#':
            return count_possibilities(str.substr(1), current_number + 1, current_number_index, required_numbers,
                                       str_completed);
        case '?':
            return count_possibilities(str, current_number, current_number_index, required_numbers, str_completed,
                                       '#') +
                   count_possibilities(str, current_number, current_number_index, required_numbers, str_completed, '.');
    }

    return 0;
}

size_t count_possibilities_with_memoization(std::string_view str, size_t current_number, size_t current_number_index,
                                            const std::vector<size_t> &required_numbers, std::string str_completed,
                                            std::vector<std::vector<std::vector<int64_t>>> &mmo, char forced_char = ' ') {
    auto& current_mmo = mmo[str.size()][current_number_index][current_number];

    char current_char = str.empty() ? ' ' : forced_char == ' ' ? str[0] : forced_char;
    str_completed += (current_char == '?' ? "" : std::string() + current_char);

    if (current_number_index == required_numbers.size()) {
        for (char c: str) {
            if (c == '#') {
                return 0;
            }
        }
        return 1;
    }

    if (current_number == required_numbers[current_number_index]) {
        if (str.empty()) {
            return current_number_index + 1 == required_numbers.size();
        }

        if (current_char == '#') {
            return 0;
        } else {
            return count_possibilities_with_memoization(str, 0, current_number_index + 1, required_numbers, str_completed, mmo, '.');
        }
    }

    if (str.empty()) return 0;

    size_t res;

    switch (current_char) {
        case '.':
            if (current_number > 0 && current_number < required_numbers[current_number_index]) {
                return 0;
            }
            return count_possibilities_with_memoization(str.substr(1), current_number, current_number_index, required_numbers,
                                             str_completed, mmo);
        case '#':
            return count_possibilities_with_memoization(str.substr(1), current_number + 1, current_number_index, required_numbers,
                                                       str_completed, mmo);
        case '?':
            if (current_mmo != -1) return current_mmo;
            res = count_possibilities_with_memoization(str, current_number, current_number_index, required_numbers, str_completed, mmo, '#') +
                    count_possibilities_with_memoization(str, current_number, current_number_index, required_numbers, str_completed, mmo, '.');
            current_mmo = res;
            return res;
    }

    return 0;
}

int first_part_2023_12() {
    std::cout << std::transform_reduce(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE),
                                       std::istream_iterator<WordDelimitedBy<'\n'>>(),
                                       0ull, std::plus<>(), [](std::string str) {
                std::string_view str_view = str;
                size_t space_index = str_view.find(' ');
                std::string_view symbols = str_view.substr(0, space_index);
                auto numbers = str_numbers<size_t, std::vector, ','>(str_view.substr(space_index + 1)).get();
                auto res = count_possibilities(symbols, 0, 0, numbers, "");
                std::cout << res << std::endl;
                return res;
            }) << std::endl;
    return EXIT_SUCCESS;
}

int second_part_2023_12() {
    std::cout << std::transform_reduce(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE),
                                       std::istream_iterator<WordDelimitedBy<'\n'>>(),
                                       0ull, std::plus<>(), [](std::string str) {
                          std::string_view str_view = str;
                          size_t space_index = str_view.find(' ');
                          std::string_view symbols = str_view.substr(0, space_index);
                          auto numbers = str_numbers<size_t, std::vector, ','>(str_view.substr(space_index + 1)).get();

                          std::string new_symbols_src(symbols.begin(), symbols.end());
                          std::string new_symbols = new_symbols_src;
                          std::vector<size_t> new_numbers(numbers);
                          new_symbols = new_symbols_src + "?"
                                        + new_symbols_src + "?"
                                        + new_symbols_src + "?"
                                        + new_symbols_src + "?"
                                        + new_symbols_src;
                          new_numbers.insert(new_numbers.end(), numbers.begin(), numbers.end());
                          new_numbers.insert(new_numbers.end(), numbers.begin(), numbers.end());
                          new_numbers.insert(new_numbers.end(), numbers.begin(), numbers.end());
                          new_numbers.insert(new_numbers.end(), numbers.begin(), numbers.end());

                          std::vector<std::vector<std::vector<int64_t>>> mmo(new_symbols.size() + 1, std::vector<std::vector<int64_t>>(new_numbers.size() + 1,std::vector<int64_t>(*std::max_element(numbers.begin(),  numbers.end()) + 1, -1)));

                          return count_possibilities_with_memoization(new_symbols,0, 0, new_numbers,"", mmo);
                      }) << std::endl;
    return EXIT_SUCCESS;
}

#endif