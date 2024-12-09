#ifndef ADVENTOFCODE2024_DAY_6_HPP
#define ADVENTOFCODE2024_DAY_6_HPP

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

int first_part_2024_6() {
    aoc_tools::matrix<char> matrix;
    point_xd<2> location{-1, -1};

    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&matrix, &location](std::string str){
        matrix.insert_line(str);

        if (location[0] == -1) {
            size_t starting_point = str.find('^');
            if (starting_point != std::string::npos) {
                location = {static_cast<long long>(starting_point), static_cast<long long>(matrix.size_y() - 1)};
            }
        }
    });

    const std::vector<point_xd<2>> directions = {
            {0, -1}, // Up
            {1, 0}, // Right
            {0, 1}, // Down
            {-1, 0} // Left
    };

    auto current_direction = directions.begin();

    while (!matrix.is_border_location(location)) {
        matrix[location] = 'X';
        location += *current_direction;

        if (matrix[location] == '#') {
            location -= *current_direction;
            ++current_direction;
            if (current_direction == directions.end()) current_direction = directions.begin();
        }
    }

    matrix[location] = 'X';

    std::cout << std::count_if(matrix.get_data().begin(), matrix.get_data().end(), [](char c) { return c == 'X'; }) << "\n";

    return EXIT_SUCCESS;
}

bool does_guard_escape(const aoc_tools::matrix<char>& matrix, point_xd<2> location, point_xd<2> new_block_location) {
    const std::vector<point_xd<2>> directions = {
            {0, -1}, // Up
            {1, 0}, // Right
            {0, 1}, // Down
            {-1, 0} // Left
    };

    auto current_direction = directions.begin();

    std::set<point_xd<3>> path; // path[2] = direction: 0,1,2,3

    while (!matrix.is_border_location(location)) {
        auto direction_number = std::distance(directions.begin(), current_direction);
        if (path.contains({location[0], location[1], direction_number}))
            return false;

        path.insert({location[0], location[1], direction_number});
        location += *current_direction;

        if (matrix[location] == '#' || location == new_block_location) {
            location -= *current_direction;
            ++current_direction;
            if (current_direction == directions.end()) current_direction = directions.begin();
        }
    }

    return true;
}

int second_part_2024_6() {
    aoc_tools::matrix<char> matrix;
    point_xd<2> location{-1, -1};

    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&matrix, &location](std::string str){
        matrix.insert_line(str);

        if (location[0] == -1) {
            size_t starting_point = str.find('^');
            if (starting_point != std::string::npos) {
                location = {static_cast<long long>(starting_point), static_cast<long long>(matrix.size_y() - 1)};
            }
        }
    });

    const auto starting_location = location;

    const std::vector<point_xd<2>> directions = {
            {0, -1}, // Up
            {1, 0}, // Right
            {0, 1}, // Down
            {-1, 0} // Left
    };

    auto current_direction = directions.begin();

    size_t options_to_lock_guard = 0;
    while (!matrix.is_border_location(location)) {
        if (matrix[location] == '.')
            options_to_lock_guard += !does_guard_escape(matrix, starting_location, location);

        matrix[location] = 'X';
        location += *current_direction;

        if (matrix[location] == '#') {
            location -= *current_direction;
            ++current_direction;
            if (current_direction == directions.end()) current_direction = directions.begin();
        }
    }

    options_to_lock_guard += !does_guard_escape(matrix, starting_location, location);

    std::cout << options_to_lock_guard << "\n";

    return EXIT_SUCCESS;
}

#endif
