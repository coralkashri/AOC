#ifndef AOC_2021_17
#define AOC_2021_17

#include <iostream>
#include <iterator>
#include <ranges>
#include <vector>
#include <bitset>
#include <list>
#include <chrono>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <sstream>
#include "../input_source.hpp"
#include "../tools/word_delimited_by.h"
#include "../tools/get_line_stream.h"
#include "../tools/std_extentions.hpp"

using range_type = std::tuple<std::pair<int, int>, std::pair<int, int>>;

void apply_drag_and_gravity(int &x, int &y) {
    x += -1 * (x != 0) * (x > 0 ? 1 : -1);
    y -= 1;
}

bool is_probe_in_range(int x, int y, const range_type &range) {
    return x >= std::get<0>(range).first && x <= std::get<0>(range).second &&
           y <= std::get<1>(range).first && y >= std::get<1>(range).second;
}

bool is_probe_before_range(int x, int y, int x_direction, int y_direction, const range_type &range) {
    bool is_x_before_range = std::abs(x - std::get<0>(range).first) > 0;
    if (x_direction > 0) {
        is_x_before_range = is_x_before_range && x < std::get<0>(range).first;
    } else {
        is_x_before_range = is_x_before_range && x > std::get<0>(range).first;
    }

    bool is_y_before_range = std::abs(y - std::get<1>(range).first) > 0;
    is_y_before_range = is_y_before_range && y > std::get<1>(range).first;
    return ((is_x_before_range && x_direction != 0) || is_y_before_range);
}

bool check_velocity(int x, int y, const range_type &range, int &highest_y_point) {
    auto current_x = x, current_y = y;
    highest_y_point = current_y;
    while (!is_probe_in_range(current_x, current_y, range) && is_probe_before_range(current_x, current_y, x, y, range)) {
        apply_drag_and_gravity(x, y);
        current_x += x;
        current_y += y;
        highest_y_point = std::max(highest_y_point, current_y);
    }
    return is_probe_in_range(current_x, current_y, range);
}

size_t find_velocity_with_the_highest_y(const range_type &range) {
    auto current_y_velocity = std::get<1>(range).second;
    int highest_y_point = current_y_velocity;
    for (auto current_x_velocity = std::get<0>(range).second; current_x_velocity >= 0; current_x_velocity--) {
        for (current_y_velocity = std::get<1>(range).second; current_y_velocity < std::get<0>(range).second; current_y_velocity++) {
            int current_highest_y_point;
            if (check_velocity(current_x_velocity, current_y_velocity, range, current_highest_y_point)) {
                highest_y_point = std::max(highest_y_point, current_highest_y_point);
            }
        }
    }
    return highest_y_point;
}

auto parse_input(std::string input) {
    auto x_section_dot = input.find('.');
    auto x_section_equal = input.find('=');
    auto x_section_end = input.find(',');
    auto start_x = std::stoi(input.substr(x_section_equal + 1, x_section_dot - x_section_equal - 1));
    auto stop_x = std::stoi(input.substr(x_section_dot + 2, x_section_end - x_section_dot - 2));

    auto y_section_dot = input.find('.', x_section_end);
    auto y_section_equal = input.find('=', x_section_end);
    auto stop_y = std::stoi(input.substr(y_section_equal + 1, y_section_dot - y_section_equal - 1));
    auto start_y = std::stoi(input.substr(y_section_dot + 2));
    return range_type{{start_x, stop_x}, {start_y, stop_y}};
}

int first_part_2021_17() {
    std::string input;
    std::getline(INPUT_SOURCE, input, '\n');
    auto range = parse_input(input);
    std::cout << find_velocity_with_the_highest_y(range) << std::endl;

    return EXIT_SUCCESS;
}

size_t find_count_of_all_available_velocities(const range_type &range) {
    size_t count_of_optional_initial_velocities = 0;
    for (auto current_x_velocity = std::get<0>(range).second; current_x_velocity >= 0; current_x_velocity--) {
        for (auto current_y_velocity = std::get<1>(range).second; current_y_velocity < std::get<0>(range).second; current_y_velocity++) {
            int current_highest_y_point;
            count_of_optional_initial_velocities += check_velocity(current_x_velocity, current_y_velocity, range, current_highest_y_point));
        }
    }
    return count_of_optional_initial_velocities;
}

int second_part_2021_17() {
    std::string input;
    std::getline(INPUT_SOURCE, input, '\n');
    auto range = parse_input(input);
    std::cout << find_count_of_all_available_velocities(range) << std::endl;

    return EXIT_SUCCESS;
}

#endif