#ifndef AOC_2023_10
#define AOC_2023_10

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include <array>
#include <set>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

namespace aoc_2023_day_10_tools {
    std::vector<std::pair<int8_t, int8_t>> relevant_neighbors(char shape) {
        switch (shape) {
            case 'F':
                return {{1, 0},
                        {0, 1}};
            case 'L':
                return {{1, 0},
                        {0, -1}};
            case '7':
                return {{-1, 0},
                        {0,  1}};
            case 'J':
                return {{-1, 0},
                        {0,  -1}};
            case '|':
                return {{0, -1},
                        {0, 1}};
            case '-':
                return {{1,  0},
                        {-1, 0}};
            case 'S':
                return {{0,  1},
                        {0,  -1},
                        {1,  0},
                        {-1, 0}};
            case '.':
            default:
                return {};
        }
    }

    size_t search_near_tiles(const aoc_tools::matrix<char> &map, aoc_tools::matrix<size_t> &steps_count_map,
                             std::vector<std::pair<size_t, size_t>> &border_points, size_t current_steps, size_t x,
                             size_t y, size_t prev_x, size_t prev_y) {
        char c = map[x, y];
        auto possible_directions = relevant_neighbors(c);
        for (auto[xd, yd]: possible_directions) {
            size_t new_x = x + xd;
            size_t new_y = y + yd;
            if (new_x == prev_x && new_y == prev_y) continue;

            if (steps_count_map[new_x, new_y] > current_steps + 1) {
                steps_count_map[new_x, new_y] = current_steps + 1;
                size_t res = search_near_tiles(map, steps_count_map, border_points, current_steps + 1, new_x, new_y, x,
                                               y);
                if (res != -1) {
                    border_points.emplace_back(x, y);
                    return res;
                }
            } else if (map[new_x, new_y] == 'S') {
                border_points.emplace_back(x, y);
                return current_steps / 2 + 1;
            }
        }

        return -1;
    }

    void clear_non_loop_characters(aoc_tools::matrix<char> &map, const std::vector<std::pair<size_t, size_t>> &loop_borders) {
        std::vector<std::pair<size_t, size_t>> sorted_border_points(loop_borders.begin(), loop_borders.end());
        std::sort(sorted_border_points.begin(), sorted_border_points.end());

        for (size_t y = 1; y < map.size_y() - 1; ++y) {
            for (size_t x = 1; x < map.size_x() - 1; ++x) {
                if (!std::binary_search(sorted_border_points.begin(), sorted_border_points.end(), std::pair{x, y})) {
                    map[x, y] = '.';
                }
            }
        }
    }

    char detect_s_shape(aoc_tools::matrix<char> &map, const std::vector<std::pair<size_t, size_t>> &border_points) {
        auto one_side = *border_points.begin();
        auto second_side = *std::next(border_points.rbegin(), 1);
        auto s_point = *border_points.rbegin();

        char c = map[one_side.first, one_side.second];
        char c1 = map[second_side.first, second_side.second];

        if (one_side.first == second_side.first) return '|';
        if (one_side.second == second_side.second) return '-';

        int8_t x_offset = one_side.first - second_side.first;
        int8_t y_offset = one_side.second - second_side.second;

        if (x_offset == y_offset && std::abs(y_offset) == 1) {
            if (s_point.second <= one_side.second || s_point.second <= second_side.second) {
                return '7';
            } else {
                return 'L';
            }
        } else /*(x_offset == -y_offset)*/ {
            if (s_point.second <= one_side.second || s_point.second <= second_side.second) {
                return 'F';
            } else {
                return 'J';
            }
        }
    }

    size_t count_dots_within_cycle(aoc_tools::matrix<char> &map, const std::vector<std::pair<size_t, size_t>> &border_points) {
        size_t count = 0;
        bool is_within = false;

        auto is_border = [](char c) {
            return c == '|' || c == 'J' || c == 'L';
        };

        for (size_t y = 0; y < map.size_y(); ++y) {
            is_within = false;
            size_t borders_count = 0;
            for (size_t x = 0; x < map.size_x(); ++x) {
                char c = map[x, y];
                if (c == 'S') {
                    c = detect_s_shape(map, border_points);
                }
                borders_count += is_border(c);
                is_within = borders_count % 2 == 1;

                if (is_within) count += c == '.';
            }
        }

        return count;
    }

    void parse_input(aoc_tools::matrix<char> &map, aoc_tools::matrix<size_t> &steps_count_map, size_t &start_x, size_t &start_y) {
        std::vector<size_t> reset_steps;
        std::vector<char> empty_line;
        start_x = -1;
        start_y = 1;

        std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE),
                      std::istream_iterator<std::string>(),
                      [&map, &steps_count_map, &start_x, &start_y, &reset_steps, &empty_line](std::string str) {
                          if (reset_steps.empty()) {
                              reset_steps.resize(str.size() + 2, -1);
                              empty_line.resize(str.size() + 2, '.');
                              map.insert_line(empty_line);
                              steps_count_map.insert_line(reset_steps);
                          }

                          map.insert_line('.' + str + '.');
                          steps_count_map.insert_line(reset_steps);
                          if (start_x == -1) {
                              if (size_t pos = str.find('S'); pos != std::string::npos) {
                                  start_x = pos + 1;
                              } else {
                                  ++start_y;
                              }
                          }
                      });

        map.insert_line(empty_line);
        steps_count_map.insert_line(reset_steps);

        steps_count_map[start_x, start_y] = 0;
    }
}

int first_part_2023_10() {
    aoc_tools::matrix<char> map;
    aoc_tools::matrix<size_t> steps_count_map;
    std::vector<std::pair<size_t, size_t>> border_points;
    size_t start_x, start_y;

    aoc_2023_day_10_tools::parse_input(map, steps_count_map, start_x, start_y);

    std::cout << aoc_2023_day_10_tools::search_near_tiles(map, steps_count_map, border_points, 0, start_x, start_y, 0, 0) << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2023_10() {
    aoc_tools::matrix<char> map;
    aoc_tools::matrix<size_t> steps_count_map;
    std::vector<std::pair<size_t, size_t>> border_points;
    size_t start_x, start_y;

    aoc_2023_day_10_tools::parse_input(map, steps_count_map, start_x, start_y);

    aoc_2023_day_10_tools::search_near_tiles(map, steps_count_map, border_points, 0, start_x, start_y, 0, 0);

    aoc_2023_day_10_tools::clear_non_loop_characters(map, border_points);

    std::cout << aoc_2023_day_10_tools::count_dots_within_cycle(map, border_points) << std::endl;

    return EXIT_SUCCESS;
}

#endif