#ifndef AOC_2023_21
#define AOC_2023_21

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include <array>
#include <set>
#include <queue>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

/*
 * Thanks to https://github.com/villuna/aoc23/wiki/A-Geometric-solution-to-advent-of-code-2023,-day-21 for the math.
 * And for https://www.reddit.com/r/adventofcode/comments/18nol3m/comment/ked9am2/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button for the edge case.
 */

struct state {
    size_t steps = 0;
    point_xd<2> location{};
};

std::vector<point_xd<2>> get_neighbors(const std::unordered_set<point_xd<2>> &points, const point_xd<2>& location) {
    std::vector<point_xd<2>> offsets = {{0,  1},
                                        {0,  -1},
                                        {1,  0},
                                        {-1, 0}};

    std::vector<point_xd<2>> neighbors;

    for (auto& offset : offsets) {
        if (points.contains(location + offset)) neighbors.push_back(location + offset);
    }

    return neighbors;
}

std::array<size_t, 2> bfs(const std::unordered_set<point_xd<2>> &points, point_xd<2> start, size_t max_steps = -1, size_t min_steps = 0) {
    std::queue<state> travelers;
    std::set<point_xd<2>> visited;
    std::array<size_t, 2> parity_counter{0, 0}; // 0 = even, 1 = odd

    travelers.push({0, start});

    while (!travelers.empty()) {
        auto [steps, location] = travelers.front();
        travelers.pop();

        if (visited.contains(location)) continue;
        visited.insert(location);

        if (steps >= min_steps) ++parity_counter[steps % 2];

        if (steps + 1 > max_steps) continue;

        for (const auto& n : get_neighbors(points, location)) {
            travelers.push({steps + 1, n});
        }
    }

    return parity_counter;
}

int first_part_2023_21() {
    std::unordered_set<point_xd<2>> points;
    point_xd<2> current_location{};
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE),
                  std::istream_iterator<std::string>(),
                          [&points, &current_location, y = 0ll] (std::string str) mutable {
        for (int64_t x = 0; x < ssize(str); ++x) {
            if (str[x] == '.') {
                points.insert({y, x});
            } else if (str[x] == 'S') {
                points.insert({y, x});
                current_location = {y, x};
            }
        }
        ++y;
    });

    std::cout << bfs(points, current_location, 64)[64 % 2] << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2023_21() {
    std::unordered_set<point_xd<2>> points;
    point_xd<2> current_location{};
    size_t dim;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE),
                  std::istream_iterator<std::string>(),
                  [&points, &current_location, &dim, y = 0ll] (std::string str) mutable {
                      dim = str.size();
                      for (int64_t x = 0; x < ssize(str); ++x) {
                          if (str[x] == '.') {
                              points.insert({y, x});
                          } else if (str[x] == 'S') {
                              points.insert({y, x});
                              current_location = {y, x};
                          }
                      }
                      ++y;
                  });

    size_t n = (26501365 - dim / 2) / dim;

    if (n != 202300) {
        std::cout << "Error!\n";
        return 1;
    }

    auto full_parities = bfs(points, current_location, -1, 0);
    auto corner_parities = bfs(points, current_location, -1, 66);

    size_t n_2 = n * n;
    size_t n_1_2 = (n + 1) * (n + 1);

    std::cout << n_1_2 * full_parities[1] + n_2 * full_parities[0] - (n + 1) * corner_parities[1] + n * corner_parities[0] - n << std::endl;

    return EXIT_SUCCESS;
}

#endif
