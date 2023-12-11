#ifndef AOC_2023_11
#define AOC_2023_11

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include <array>
#include <set>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

using universe_t = std::vector<std::string>;

bool is_empty_row(std::string_view str) {
    return std::ranges::all_of(str, [] (char c) { return c == '.'; });
}

bool is_empty_columns(const universe_t& universe, size_t column) {
    return std::ranges::all_of(universe, [column](const auto& row) { return row[column] == '.'; });
}

void fill_universe(universe_t& universe) {
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE),
                  std::istream_iterator<std::string>(),
                  [&universe] (std::string str) mutable {
                      universe.push_back(str);
                  });
}

void fill_empty_rows_and_columns(const universe_t& universe, std::set<size_t> &empty_rows, std::set<size_t>& empty_columns) {
    for (size_t i = 0; i < universe.size(); ++i) {
        if (is_empty_row(universe[i])) empty_rows.insert(i);
    }

    for (size_t i = 0; i < universe[0].size(); ++i) {
        if (is_empty_columns(universe, i)) empty_columns.insert(i);
    }
}

std::vector<point_xd<2>> get_galaxies_coordinates(const universe_t& universe) {
    std::vector<point_xd<2>> galaxies_coordinates;
    int64_t line = 0;
    for (auto& row : universe) {
        int64_t col = 0;
        for (char c: row) {
            if (c == '#') {
                galaxies_coordinates.push_back({col, line});
            }
            ++col;
        }
        ++line;
    }
    return galaxies_coordinates;
}

template<size_t ExpendDistance>
size_t sum_distances(const std::vector<point_xd<2>> &galaxies_coordinates, const std::set<size_t> &empty_rows, const std::set<size_t> &empty_columns) {
    size_t sum = 0;

    for (size_t i = 0; i < galaxies_coordinates.size() - 1; ++i) {
        for (size_t j = i + 1; j < galaxies_coordinates.size(); ++j) {
            sum += galaxies_coordinates[i].simple_distance(galaxies_coordinates[j]);
            for (auto it = empty_rows.upper_bound(std::min(galaxies_coordinates[i][1], galaxies_coordinates[j][1])); it != empty_rows.end(); ++it) {
                if (*it > std::max(galaxies_coordinates[i][1], galaxies_coordinates[j][1])) break;
                sum += ExpendDistance;
            }

            for (auto it = empty_columns.upper_bound(std::min(galaxies_coordinates[i][0], galaxies_coordinates[j][0])); it != empty_columns.end(); ++it) {
                if (*it > std::max(galaxies_coordinates[i][0], galaxies_coordinates[j][0])) break;
                sum += ExpendDistance;
            }
        }
    }
    return sum;
}

template<size_t ExpendDistance>
size_t solve() {
    universe_t universe;
    std::vector<point_xd<2>> galaxies_coordinates;
    std::set<size_t> empty_rows, empty_columns;

    fill_universe(universe);
    fill_empty_rows_and_columns(universe, empty_rows, empty_columns);
    galaxies_coordinates = get_galaxies_coordinates(universe);

    return sum_distances<ExpendDistance>(galaxies_coordinates, empty_rows, empty_columns);
}

int first_part_2023_11() {
    std::cout << solve<1>() << std::endl;
    return EXIT_SUCCESS;
}

int second_part_2023_11() {
    std::cout << solve<999999>() << std::endl;
    return EXIT_SUCCESS;
}

#endif