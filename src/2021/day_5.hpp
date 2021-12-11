#ifndef AOC_2021_5
#define AOC_2021_5
#include <iostream>
#include <chrono>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>
#include <ranges>
#include "../input_source.hpp"
#include "../tools/word_delimited_by.h"

int first_part_2021_5() {
    std::map<std::pair<size_t, size_t>, size_t> occupied_points;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (auto line) {
        auto arrow_it = line.find(" -> ");
        auto start_pos = line.substr(0, arrow_it);
        auto end_pos = line.substr(arrow_it + 4, line.size() - arrow_it - 4);

        auto start_pos_delimiter_it = start_pos.find(',');
        auto x1 = std::stoll(start_pos.substr(0, start_pos_delimiter_it));
        auto y1 = std::stoll(start_pos.substr(start_pos_delimiter_it + 1, start_pos.size() - start_pos_delimiter_it - 1));

        auto end_pos_delimiter_it = end_pos.find(',');
        auto x2 = std::stoll(end_pos.substr(0, end_pos_delimiter_it));
        auto y2 = std::stoll(end_pos.substr(end_pos_delimiter_it + 1, end_pos.size() - end_pos_delimiter_it - 1));

        if (x1 == x2 || y1 == y2) { // Remove this condition for the second part
            auto x_direction = (x1 - x2) < 0 ? 1 : -1;
            auto y_direction = (y1 - y2) < 0 ? 1 : -1;
            while (x1 != x2 || y1 != y2) {
                occupied_points[std::pair{x1, y1}]++;
                x1 += x_direction * (x1 != x2);
                y1 += y_direction * (y1 != y2);
            }
            occupied_points[std::pair{x1, y1}]++;
        }
    });

    std::cout << std::count_if(occupied_points.begin(), occupied_points.end(), [] (auto point) {
        return point.second >= 2;
    }) << std::endl;

    return EXIT_SUCCESS;
}

#endif
