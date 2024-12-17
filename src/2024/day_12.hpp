#ifndef ADVENTOFCODE2024_DAY_12_HPP
#define ADVENTOFCODE2024_DAY_12_HPP

#include <iostream>
#include <algorithm>
#include <iterator>
#include <map>
#include <ranges>
#include <numeric>
#include <regex>
#include <cmath>

#include "../input_source.hpp"
#include "../tools/str_number.hpp"
#include "../tools/word_delimited_by.h"
#include "../tools/matrix.hpp"
#include "../tools/point.hpp"
#include <set>
#include "../tools/range.hpp"

std::tuple<size_t, size_t> calculate_total_price(const aoc_tools::matrix<char>& garden, point_xd<2> location, std::set<point_xd<2>>& visited) {
    static const std::vector<point_xd<2>> directions = {
            {0, 1},
            {0, -1},
            {1, 0},
            {-1, 0}
    };

    if (visited.contains(location)) return {0, 0};

    visited.insert(location);

    size_t area = 1;
    size_t perimeter = 0;

    for (auto d : directions) {
        if (garden[location + d] == garden[location]) {
            auto [d_area, d_perimeter] = calculate_total_price(garden, location + d, visited);
            area += d_area;
            perimeter += d_perimeter;
        } else {
            ++perimeter;
        }
    }
    return {area, perimeter};
}

size_t scan_garden(const aoc_tools::matrix<char>& garden) {
    std::set<point_xd<2>> visited;
    size_t total_pricing = 0;
    for (long long x = 1; x < garden.size_x() - 1; ++x) {
        for (long long y = 1; y < garden.size_y() - 1; ++y) {
            auto [area, perimeter] = calculate_total_price(garden, point_xd<2>{x, y}, visited);
            total_pricing += area * perimeter;
        }
    }

    return total_pricing;
}

int first_part_2024_12() {
    aoc_tools::matrix<char> garden;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&garden](std::string str) {
        garden.insert_line("~" + str + "~");
    });
    garden.insert_padding_lines('~');

    std::cout << scan_garden(garden) << "\n";
    return EXIT_SUCCESS;
}

std::tuple<size_t, size_t> calculate_total_price_with_discount(const aoc_tools::matrix<char>& garden, point_xd<2> location, std::set<point_xd<2>>& visited) {
    point_tools::directions all_directions;

    if (visited.contains(location)) return {0, 0};

    std::stack<point_xd<2>> next_locations;
    next_locations.push(location);
    size_t area = 0;
    size_t slides = 0;

    const auto up_d = all_directions.get_direction(point_tools::up);
    const auto right_d = all_directions.get_direction(point_tools::right);
    const auto down_d = all_directions.get_direction(point_tools::down);
    const auto left_d = all_directions.get_direction(point_tools::left);
    const auto left_up_d = all_directions.get_direction(point_tools::left_up);
    const auto left_down_d = all_directions.get_direction(point_tools::left_down);
    const auto right_up_d = all_directions.get_direction(point_tools::right_up);

    while (!next_locations.empty()) {
        location = next_locations.top();
        next_locations.pop();

        if (visited.contains(location)) continue;
        visited.insert(location);

        ++area;

        if (garden[location + up_d] != garden[location]) {
            if (garden[location + left_d] != garden[location] || garden[location + left_up_d] == garden[location]) {
                slides++;
            }
        }

        if (garden[location + right_d] != garden[location]) {
            if (garden[location + up_d] != garden[location] || garden[location + right_up_d] == garden[location]) {
                slides++;
            }
        }

        if (garden[location + down_d] != garden[location]) {
            if (garden[location + left_d] != garden[location] || garden[location + left_down_d] == garden[location]) {
                slides++;
            }
        }

        if (garden[location + left_d] != garden[location]) {
            if (garden[location + up_d] != garden[location] || garden[location + left_up_d] == garden[location]) {
                slides++;
            }
        }

        std::vector<point_xd<2>> border_locations;
        for (auto direction : all_directions.get_basic_directions({})) {
            if (garden[location + direction] == garden[location] && !visited.contains(location + direction)) {
                next_locations.push(location + direction);
            }
        }
    }

    return {area, slides};
}

size_t scan_garden_with_discount(const aoc_tools::matrix<char>& garden) {
    std::set<point_xd<2>> visited;
    size_t total_pricing = 0;
    for (long long x = 1; x < garden.size_x() - 1; ++x) {
        for (long long y = 1; y < garden.size_y() - 1; ++y) {
            auto [area, perimeter] = calculate_total_price_with_discount(garden, point_xd<2>{x, y}, visited);
            total_pricing += area * perimeter;
        }
    }

    return total_pricing;
}

int second_part_2024_12() {
    aoc_tools::matrix<char> garden;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&garden](std::string str) {
        garden.insert_line("~" + str + "~");
    });
    garden.insert_padding_lines('~');

    std::cout << scan_garden_with_discount(garden) << "\n";

    return EXIT_SUCCESS;
}

#endif
