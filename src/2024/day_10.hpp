#ifndef ADVENTOFCODE2024_DAY_10_HPP
#define ADVENTOFCODE2024_DAY_10_HPP

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

using matrix_t = aoc_tools::matrix<std::pair<char, bool>>;

size_t travel_helper(matrix_t& matrix, point_xd<2> location) {
    matrix[location].second = true;
    size_t neighbors_sum = 0;

    static const std::vector<point_xd<2>> directions = {
            {0, 1},
            {0, -1},
            {1, 0},
            {-1, 0}
    };

    for (auto direction : directions) {
        auto next_location = location + direction;
        if (matrix[next_location].first - matrix[location].first == 1 && !matrix[next_location].second) {
            neighbors_sum += travel_helper(matrix, next_location);
        }
    }

    return neighbors_sum + (matrix[location].first == '9');
}

size_t travel_helper2(matrix_t matrix, point_xd<2> location) { // TODO Check optimization of using std::set of visited dots (by value) instead of copy the map each call
    matrix[location].second = true;
    size_t neighbors_sum = 0;

    static const std::vector<point_xd<2>> directions = {
            {0, 1},
            {0, -1},
            {1, 0},
            {-1, 0}
    };

    for (auto direction : directions) {
        auto next_location = location + direction;
        if (matrix[next_location].first - matrix[location].first == 1 && !matrix[next_location].second) {
            neighbors_sum += travel_helper2(matrix, next_location);
        }
    }

    return neighbors_sum + (matrix[location].first == '9');
}

size_t travel(matrix_t matrix, point_xd<2> location) {
    return travel_helper(matrix, location);
}

int first_part_2024_10() {
    matrix_t matrix;
    std::vector<point_xd<2>> start_locations;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&, row = 1ll](std::string str) mutable {
        static std::vector<std::pair<char, bool>> line(str.size() + 2, {'Y', false});
        auto line_it = line.begin() + 1;
        long long column = 1;
        for (char c : str) {
            line_it++->first = c;
            if (c == '0') start_locations.push_back({column, row});
            ++column;
        }

        matrix.insert_line(line);

        ++row;
    });

    matrix.insert_padding_lines({'Y', false});

    size_t sum = 0;
    for (auto location : start_locations) {
        sum += travel(matrix, location);
    }

    std::cout << sum << "\n";

    return EXIT_SUCCESS;
}

int second_part_2024_10() {
    matrix_t matrix;
    std::vector<point_xd<2>> start_locations;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&, row = 1ll](std::string str) mutable {
        static std::vector<std::pair<char, bool>> line(str.size() + 2, {'Y', false});
        auto line_it = line.begin() + 1;
        long long column = 1;
        for (char c : str) {
            line_it++->first = c;
            if (c == '0') start_locations.push_back({column, row});
            ++column;
        }

        matrix.insert_line(line);

        ++row;
    });

    matrix.insert_padding_lines({'Y', false});

    size_t sum = 0;
    for (auto location : start_locations) {
        sum += travel_helper2(matrix, location);
    }

    std::cout << sum << "\n";

    return EXIT_SUCCESS;
}

#endif
