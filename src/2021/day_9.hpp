#ifndef AOC_2021_9
#define AOC_2021_9

#include <iostream>
#include <iterator>
#include <vector>
#include <list>
#include <chrono>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>
#include <ranges>
#include <numeric>
#include <cmath>
#include "../input_source.hpp"

bool is_lowest_between_neighbors(const auto &matrix, auto i, auto j) {
    auto current_number = matrix.at(i).at(j);
    return current_number < matrix.at(i - 1).at(j) && current_number < matrix.at(i + 1).at(j) &&
           current_number < matrix.at(i).at(j - 1) && current_number < matrix.at(i).at(j + 1);
}

size_t search_neighbors_within_9_section(const auto &matrix, auto i, auto j, std::vector<std::pair<int, int>> &visited_points) {
    auto current_number = matrix.at(i).at(j);
    if (current_number == 9 || std::find(visited_points.begin(), visited_points.end(), std::pair{i, j}) != visited_points.end()) return 0;
    visited_points.template emplace_back(std::pair{i, j});
    size_t res = search_neighbors_within_9_section(matrix, i + 1, j, visited_points);
    res += search_neighbors_within_9_section(matrix, i - 1, j, visited_points);
    res += search_neighbors_within_9_section(matrix, i, j + 1, visited_points);
    res += search_neighbors_within_9_section(matrix, i, j - 1, visited_points);
    return res + 1;
}

int first_part_2021_9() {
    std::vector<std::vector<int>> matrix;
    std::vector<int> low_numbers;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [&] (auto line) {
        matrix.template emplace_back();
        line = "9" + line + "9"; // Add padding
        std::transform(line.begin(), line.end(), std::back_inserter(matrix.back()), [] (char number) {
            return number - '0';
        });
    });
    matrix.insert(matrix.begin(), std::vector<int>(matrix.at(0).size(), 9)); // Add padding
    matrix.emplace_back(matrix.at(0).size(), 9); // Add padding
    for (size_t i = 1; i < matrix.size() - 1; i++) {
        for (size_t j = 1; j < matrix.at(i).size() - 1; j++) {
             if (is_lowest_between_neighbors(matrix, i, j)) low_numbers.emplace_back(matrix.at(i).at(j));
        }
    }

    std::cout << "Result: " << std::transform_reduce(low_numbers.begin(), low_numbers.end(), 0, std::plus<>{}, [] (auto number) { return 1 + number; }) << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2021_9() {
    std::vector<std::vector<int>> matrix;
    std::vector<std::pair<int, int>> low_numbers_locations;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [&] (auto line) {
        matrix.template emplace_back();
        line = "9" + line + "9"; // Add padding
        std::transform(line.begin(), line.end(), std::back_inserter(matrix.back()), [] (char number) {
            return number - '0';
        });
    });
    matrix.insert(matrix.begin(), std::vector<int>(matrix.at(0).size(), 9)); // Add padding
    matrix.emplace_back(matrix.at(0).size(), 9); // Add padding
    for (size_t i = 1; i < matrix.size() - 1; i++) {
        for (size_t j = 1; j < matrix.at(i).size() - 1; j++) {
            if (is_lowest_between_neighbors(matrix, i, j)) low_numbers_locations.emplace_back(std::pair{i, j});
        }
    }

    int basin, basin1, basin2;
    basin = basin1 = basin2 = 0;
    for (auto low_number_location : low_numbers_locations) {
        std::vector<std::pair<int, int>> visited_places;
        auto current_basin = search_neighbors_within_9_section(matrix, low_number_location.first, low_number_location.second, visited_places);
        if (current_basin > basin) basin2 = std::exchange(basin1, std::exchange(basin, current_basin));
        else if (current_basin > basin1) basin2 = std::exchange(basin1, current_basin);
        else if (current_basin > basin2) basin2 = current_basin;
    }

    std::cout << "Result: " << basin * basin1 * basin2 << std::endl;

    return EXIT_SUCCESS;
}

#endif