#ifndef ADVENTOFCODE2024_DAY_16_HPP
#define ADVENTOFCODE2024_DAY_16_HPP

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
#include <thread>
#include "../tools/range.hpp"

#include "../tools/linear_algebra/all.hpp"

using matrix_t = aoc_tools::matrix<char>;

struct path {
    point_xd<2> location;
    size_t score;
    point_tools::direction_names_advanced direction;
    std::vector<point_xd<2>> visited_locations;

    bool operator<(const path &ref) const {
        return std::tie(score, location, direction) < std::tie(ref.score, ref.location, ref.direction);
    }
};

size_t next_possible_directions_count(const matrix_t &maze, const path &location) {
    size_t count = 0;
    point_tools::directions all_directions;
    for (auto d: point_tools::directions_iterator<point_tools::direction_names_advanced, true>()) {
        if (d != (location.direction + 4) % 8) { // no backwards
            point_xd<2> next_location = location.location;
            count += maze[next_location + all_directions.get_direction(d)] != '#';
        }
    }
    return count;
}

std::vector<path> next_possible_positions(const matrix_t &maze, const path &location) {
    std::vector<path> res;
    point_tools::directions all_directions;
    for (auto d: point_tools::directions_iterator<point_tools::direction_names_advanced, true>()) {
        if (d != (location.direction + 4) % 8) { // no backwards
            point_xd<2> next_location = location.location;
            auto step = all_directions.get_direction(d);
            size_t additional_score = d == location.direction ? 0 : 1000;
            auto visited_locations = location.visited_locations;
            while (maze[next_location + step] != '#') {
                next_location += step;
                visited_locations.push_back(next_location);
                ++additional_score;
                if (next_possible_directions_count(maze, {.location = next_location, .direction = d}) > 1) {
                    break;
                }
            }

            if (next_location != location.location) {
                res.push_back({.location = next_location, .score = location.score + additional_score, .direction = d, .visited_locations = visited_locations});
            }
        }
    }
    return res;
}

size_t travel(const matrix_t &maze, point_xd<2> starting_location) {
    std::priority_queue<path> next_paths;
    next_paths.push({starting_location, 0, point_tools::direction_names_advanced::east});
    std::map<std::pair<point_xd<2>, point_tools::direction_names_advanced>, size_t> memo;

    while (!next_paths.empty()) {
        auto p = next_paths.top();
        next_paths.pop();

        if (auto it = memo.find({p.location, p.direction}); it == memo.end() || it->second > p.score) {
            memo[{p.location, p.direction}] = p.score;
        } else {
            continue;
        }

        auto next = next_possible_positions(maze, p);
        for (const auto& n : next) {
            next_paths.push(n);
        }
    }

    point_xd<2> exit_point = {static_cast<long long>(maze.size_x()) - 2ll, 1};
    auto north_points = memo[{exit_point, point_tools::direction_names_advanced::north}];
    auto east_points = memo[{exit_point, point_tools::direction_names_advanced::east}];
    return std::min(north_points ? north_points : 99999999, east_points ? east_points : 99999999);
}

int first_part_2024_16() {
    matrix_t maze;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&maze](std::string str) {
        maze.insert_line(str);
    });

    std::cout << travel(maze, {1, static_cast<long long>(maze.size_y()) - 2}) << "\n";

    return EXIT_SUCCESS;
}

size_t find_possible_best_paths(const matrix_t &maze, point_xd<2> starting_location) {
    std::priority_queue<path> next_paths;
    next_paths.push({starting_location, 0, point_tools::direction_names_advanced::east, {starting_location}});
    std::map<std::pair<point_xd<2>, point_tools::direction_names_advanced>, size_t> memo;
    point_xd<2> exit_point = {static_cast<long long>(maze.size_x()) - 2ll, 1};
    std::map<size_t, std::set<point_xd<2>>> best_paths;

    while (!next_paths.empty()) {
        auto p = next_paths.top();
        next_paths.pop();

        if (!best_paths.empty()) {
            if (p.score > best_paths.begin()->first) continue;
        }

        if (p.location == exit_point) {
            best_paths[p.score].insert(p.visited_locations.begin(), p.visited_locations.end());
            continue;
        }

        if (auto it = memo.find({p.location, p.direction}); it == memo.end() || it->second >= p.score) {
            memo[{p.location, p.direction}] = p.score;
        } else {
            continue;
        }

        auto next = next_possible_positions(maze, p);
        for (auto& n : next) {
            next_paths.push(n);
        }
    }

    /*// Print maze with marked visited locations:
    maze.for_each([p = best_paths.begin()->second, last_y = -1ll](char c, size_t x, size_t y) mutable {
        if (last_y != y) {
            last_y = y;
            std::cout << "\n";
        }

        if (p.contains({(long long)x, (long long)y})) std::cout << "O";
        else std::cout << c;
    });*/

    return best_paths.begin()->second.size();
}

int second_part_2024_16() {
    matrix_t maze;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&maze](std::string str) {
        maze.insert_line(str);
    });

    std::cout << find_possible_best_paths(maze, {1, static_cast<long long>(maze.size_y()) - 2}) << "\n";


    return EXIT_SUCCESS;
}

#endif
