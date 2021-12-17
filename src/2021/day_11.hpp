#ifndef AOC_2021_11
#define AOC_2021_11

#include <iostream>
#include <iterator>
#include <ranges>
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

size_t increase_oct_level(auto &matrix, auto i, auto j, bool is_flashed, std::vector<std::pair<size_t, size_t>> &visited_points) {
    size_t flashes_count = 0;
    auto &current_number = matrix.at(i).at(j);
    if (current_number < 0 || std::find(visited_points.begin(), visited_points.end(), std::pair{i, j}) != visited_points.end()) return flashes_count;
    if (++current_number > 9) {
        visited_points.emplace_back(std::pair{i, j});
        current_number = 0;
        flashes_count++;
        for (int i_offset = -1; i_offset <= 1; i_offset++) {
            for (int j_offset = -1; j_offset <= 1; j_offset++) {
                if (i_offset == 0 && j_offset == 0) continue;
                flashes_count += increase_oct_level(matrix, i + i_offset, j + j_offset, true, visited_points);
            }
        }
    }
    return flashes_count;
}

size_t next_step(auto &mat) {
    size_t flashes_count = 0;
    std::vector<std::pair<size_t, size_t>> visited_points;
    for (size_t i = 0; i < mat.size(); i++) {
        for (size_t j = 0; j < mat.at(i).size(); j++) {
            flashes_count += increase_oct_level(mat, i, j, false, visited_points);
        }
    }
    return flashes_count;
}

int first_part_2021_11() {
    std::vector<std::vector<int>> mat;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [&] (auto line) {
        mat.template emplace_back();
        line = "-" + line + "-"; // Add padding
        std::transform(line.begin(), line.end(), std::back_inserter(mat.back()), [] (char number) {
            return number - '0';
        });
    });
    mat.insert(mat.begin(), std::vector<int>(mat.at(0).size(), -3)); // Add padding
    mat.emplace_back(mat.at(0).size(), -3); // Add padding

    size_t res = 0;
    for (size_t i = 0; i < 100; i++)
        res += next_step(mat);

    std::cout << "Result: " << res << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2021_11() {
    std::vector<std::vector<int>> mat;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [&] (auto line) {
        mat.template emplace_back();
        line = "-" + line + "-"; // Add padding
        std::transform(line.begin(), line.end(), std::back_inserter(mat.back()), [] (char number) {
            return number - '0';
        });
    });
    mat.insert(mat.begin(), std::vector<int>(mat.at(0).size(), -3)); // Add padding
    mat.emplace_back(mat.at(0).size(), -3); // Add padding

    size_t res = 1;
    size_t octopus_count = ((mat.size() - 2) * (mat.at(0).size() - 2));
    while (next_step(mat) < octopus_count) res++;

    std::cout << "Result: " << res << std::endl;

    return EXIT_SUCCESS;
}

#endif