#ifndef ADVENTOFCODE2024_DAY_1_HPP
#define ADVENTOFCODE2024_DAY_1_HPP

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

using matrix_type = aoc_tools::matrix<char>;

bool find_next_letters(const matrix_type& matrix, const point_xd<2>& prev_location, const point_xd<2>& direction, char prev_letter = 'X') {
    const char letter_to_find = [prev_letter] {
        switch (prev_letter) {
            case 'X': return 'M';
            case 'M': return 'A';
            case 'A': return 'S';
            default: exit(1);
        }
    }();

    auto location = prev_location + direction;

    if (matrix[location] == letter_to_find) {
        if (letter_to_find == 'S') return true;
        return find_next_letters(matrix, location, direction, letter_to_find);
    }

    return false;
}

template <typename FuncT>
size_t scan_matrix(const matrix_type& matrix, char letter, FuncT&& func) {
    size_t counter = 0;

    for (int64_t y = 1; y < matrix.size_y() - 1; ++y) {
        for (int64_t x = 1; x < matrix.size_x() - 1; ++x) {
            if (matrix[x, y] == letter) {
                counter += func(matrix, x, y);
            }
        }
    }

    return counter;
}

int first_part_2024_4() {
    matrix_type m;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&m](std::string str) {
        m.insert_line("." + str + ".");
    });
    m.insert_padding_lines(',');

    std::cout << scan_matrix(m, 'X', [](const auto& matrix, int64_t x, int64_t y) {
        size_t xmas_counter = 0;
        for (int8_t xd = -1; xd <= 1; ++xd) {
            for (int8_t yd = -1; yd <= 1; ++yd) {
                if (xd == 0 && yd == 0) continue;
                xmas_counter += find_next_letters(matrix, {x, y}, {xd, yd});
            }
        }
        return xmas_counter;
    }) << "\n";

    return EXIT_SUCCESS;
}

bool find_ms_around_a(const matrix_type& matrix, int64_t x, int64_t y) {
    point_xd<2> a_location{x, y};

    auto top_left_corner = a_location + point_xd<2>{-1, -1};
    auto top_right_corner = a_location + point_xd<2>{1, -1};
    auto bottom_left_corner = a_location + point_xd<2>{-1, 1};
    auto bottom_right_corner = a_location + point_xd<2>{1, 1};

    if (matrix[top_left_corner] == 'M' && matrix[top_right_corner] == 'M' && matrix[bottom_left_corner] == 'S' && matrix[bottom_right_corner] == 'S') return true;
    if (matrix[top_left_corner] == 'M' && matrix[top_right_corner] == 'S' && matrix[bottom_left_corner] == 'M' && matrix[bottom_right_corner] == 'S') return true;
    if (matrix[top_left_corner] == 'S' && matrix[top_right_corner] == 'M' && matrix[bottom_left_corner] == 'S' && matrix[bottom_right_corner] == 'M') return true;
    if (matrix[top_left_corner] == 'S' && matrix[top_right_corner] == 'S' && matrix[bottom_left_corner] == 'M' && matrix[bottom_right_corner] == 'M') return true;

    return false;
}

int second_part_2024_4() {
    matrix_type m;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&m](std::string str) {
        m.insert_line("." + str + ".");
    });
    m.insert_padding_lines(',');

    std::cout << scan_matrix(m, 'A', find_ms_around_a) << "\n";

    return EXIT_SUCCESS;
}

#endif //ADVENTOFCODE2024_DAY_1_HPP
