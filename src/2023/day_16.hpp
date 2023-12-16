#ifndef AOC_2023_16
#define AOC_2023_16

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include <array>
#include <set>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

point_xd<2> get_next_location(const point_xd<2> &current_location, int degrees) {
    point_xd<2> next_location = current_location;
    switch (degrees % 360) {
        case 180:
            next_location[1]++; // Right
            break;
        case 270:
            next_location[0]++; // Down
            break;
        case 0:
            next_location[1]--; // Left
            break;
        case 90:
            next_location[0]--; // Up
            break;
    }
    return next_location;
}

std::vector<int>
get_next_move_degrees(const std::vector<std::string> &map, const point_xd<2> &current_location, int degrees) {
    char current_elem = map[current_location[0]][current_location[1]];
    switch (current_elem) {
        case '/': // 180 -> 90, 270 -> 360, 360/0 -> 270, 90 -> 180
            return {degrees == 180 || degrees == 0 ? degrees - 90 : degrees + 90};
        case '\\': // 180 -> 270, 270 -> 180, 0 -> 90, 90 -> 0
            return {degrees == 180 || degrees == 0 ? degrees + 90 : degrees - 90};
        case '-':
            return degrees == 270 || degrees == 90 ? std::vector<int>{180, 0} : std::vector<int>{degrees};
        case '|':
            return degrees == 180 || degrees == 0 ? std::vector<int>{270, 90} : std::vector<int>{degrees};
        default:
            return {degrees};
    }
}

void scan_map(const std::vector<std::string> &map, const point_xd<2> &current_location, int degrees,
              std::set<std::pair<point_xd<2>, int>> &visited_locations, std::set<point_xd<2>> &pure_visited_locations) {
    if (current_location[0] < 0 || current_location[1] < 0 || current_location[0] >= map.size() ||
        current_location[1] >= map[0].size())
        return;
    if (visited_locations.find({current_location, degrees}) != visited_locations.end()) return;
    visited_locations.insert({current_location, degrees});
    pure_visited_locations.insert(current_location);
    auto next_degrees = get_next_move_degrees(map, current_location, degrees);
    for (int d: next_degrees) {
        if (d < 0) d += 360;
        d %= 360;
        auto next_location = get_next_location(current_location, d);
        scan_map(map, next_location, d, visited_locations, pure_visited_locations);
    }
}

std::vector<std::string> parse_input() {
    std::vector<std::string> map;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(),
                  [&map](std::string str) {
                      map.push_back(str);
                  });
    return map;
}

int first_part_2023_16() {
    auto map = parse_input();
    std::set<std::pair<point_xd<2>, int>> visited_locations;
    std::set<point_xd<2>> pure_visited_locations;
    scan_map(map, {0, 0}, 180, visited_locations, pure_visited_locations);

    /*for (int64_t i = 0; i < map.size(); ++i) {
        for (int64_t j = 0; j < map[0].size(); ++j) {
            std::cout << map[i][j];
        }
        std::cout << "\n";
    }

    std::cout << "\n";
    std::cout << "\n";

    for (int64_t i = 0; i < map.size(); ++i) {
        for (int64_t j = 0; j < map[0].size(); ++j) {
            if (visited_locations.find({{i, j}, 90}) != visited_locations.end() ||
                    visited_locations.find({{i, j}, 0}) != visited_locations.end() ||
                    visited_locations.find({{i, j}, 270}) != visited_locations.end() ||
                    visited_locations.find({{i, j}, 180}) != visited_locations.end())
                std::cout << "#";
            else std::cout << map[i][j];
        }
        std::cout << "\n";
    }*/

    std::cout << pure_visited_locations.size() << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2023_16() {
    size_t max_energy = 0;
    auto map = parse_input();

    for (int64_t x = 0; x < map[0].size(); ++x) {
        std::set<std::pair<point_xd<2>, int>> visited_locations;
        std::set<point_xd<2>> pure_visited_locations;
        scan_map(map, {0, x}, 270, visited_locations, pure_visited_locations);
        max_energy = std::max(max_energy, pure_visited_locations.size());
    }

    for (int64_t x = 0; x < map[0].size(); ++x) {
        std::set<std::pair<point_xd<2>, int>> visited_locations;
        std::set<point_xd<2>> pure_visited_locations;
        scan_map(map, {ssize(map) - 1, x}, 90, visited_locations, pure_visited_locations);
        max_energy = std::max(max_energy, pure_visited_locations.size());
    }

    for (int64_t y = 0; y < map.size(); ++y) {
        std::set<std::pair<point_xd<2>, int>> visited_locations;
        std::set<point_xd<2>> pure_visited_locations;
        scan_map(map, {y, 0}, 180, visited_locations, pure_visited_locations);
        max_energy = std::max(max_energy, pure_visited_locations.size());
    }

    for (int64_t y = 0; y < map.size(); ++y) {
        std::set<std::pair<point_xd<2>, int>> visited_locations;
        std::set<point_xd<2>> pure_visited_locations;
        scan_map(map, {y, ssize(map[0]) - 1}, 0, visited_locations, pure_visited_locations);
        max_energy = std::max(max_energy, pure_visited_locations.size());
    }

    std::cout << max_energy << std::endl;

    return EXIT_SUCCESS;
}

#endif