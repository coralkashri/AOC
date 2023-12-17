#ifndef AOC_2023_17
#define AOC_2023_17

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

struct next_point {
    int64_t y, x;
    std::pair<int, int> direction;
    uint8_t times_in_current_direction;
    int64_t heatLoss = 0;
    int64_t y_parent, x_parent;

    bool operator>(const next_point &ref) const {
        return heatLoss > ref.heatLoss;
    }
};

void trace_path(const auto &mat, const std::vector<next_point>& points) {
    std::stack<next_point> path;
    auto current = points.back();

    while (current.y != 0 || current.x != 0) {
        path.push(current);
        current = *std::find_if(points.rbegin(), points.rend(), [yp = current.y_parent, xp = current.x_parent](const next_point& p) {
            return p.y == yp && p.x == xp;
        });
    }

    while (!path.empty()) {
        current = path.top();
        path.pop();
        std::cout << "(" << current.y << ", " << current.x << " [" << current.heatLoss << "]" ")" << " -> ";
    }

    std::cout << std::endl;
}

template <size_t MinStepsInDirection, size_t MaxStepsInDirection>
void update_neighbors(const auto &mat, auto &neighbors, auto &visited, const next_point &current) {
    std::vector<std::pair<int, int>> possible_directions = {{0,  1},
                                                            {0,  -1},
                                                            {1,  0},
                                                            {-1, 0}};
    neighbors.clear();

    for (auto &d: possible_directions) {
        std::pair<int, int> current_pure_direction;
        current_pure_direction.first = current.direction.first == 0 ? 0 : current.direction.first / current.direction.first * (current.direction.first < 0 ? -1 : 1);
        current_pure_direction.second = current.direction.second == 0 ? 0 : current.direction.second / current.direction.second * (current.direction.second < 0 ? -1 : 1);

        bool is_same_direction = d.first == current_pure_direction.first && d.second == current_pure_direction.second;
        bool is_opposite_direction = d.first == -current_pure_direction.first && d.second == -current_pure_direction.second;

        if (!is_same_direction) {
            d.first *= MinStepsInDirection;
            d.second *= MinStepsInDirection;
        }
        size_t steps_count = std::abs(d.first + d.second);

        if (is_opposite_direction) continue;
        if (is_same_direction && current.times_in_current_direction + 1 > MaxStepsInDirection) continue;
        if (current.y + d.first < 0 || current.y + d.first >= mat.size()) continue;
        if (current.x + d.second < 0 || current.x + d.second >= mat[0].size()) continue;
        neighbors.push_back({.y = current.y + d.first,
                                    .x = current.x + d.second,
                                    .direction = d,
                                    .times_in_current_direction = (uint8_t)(is_same_direction ? current.times_in_current_direction + steps_count : steps_count)});
    }
}

template <size_t MinStepsInDirection, size_t MaxStepsInDirection>
std::size_t shortest_path_value(auto &points) {
    std::vector<next_point> neighbors;
    std::priority_queue<next_point, std::vector<next_point>, std::greater<>> next_to_visit;
    std::set<std::tuple<int64_t, int64_t, int, int, uint8_t>> visited;
    next_point current = {.y = 0, .x = 0, .direction = {0, 0}, .times_in_current_direction = 0, .heatLoss = 0};
    next_to_visit.push(current);
    while (!next_to_visit.empty()) {
        current = next_to_visit.top();
        next_to_visit.pop();

        if (current.y == points.size() - 1 && current.x == points[0].size() - 1) {
            return current.heatLoss;
        }

        if (visited.count({current.y, current.x, current.direction.first, current.direction.second, current.times_in_current_direction})) continue;
        visited.insert({current.y, current.x, current.direction.first, current.direction.second, current.times_in_current_direction});
        update_neighbors<MinStepsInDirection, MaxStepsInDirection>(points, neighbors, visited, current);

        for (auto &point: neighbors) {
            point.heatLoss = current.heatLoss;
            size_t steps_count = std::abs(point.direction.first + point.direction.second);
            for (size_t i = 0; i < steps_count; ++i) {
                auto new_y = point.y - point.direction.first / steps_count * i;
                auto new_x = point.x - point.direction.second / steps_count * i;
                point.heatLoss += points[new_y][new_x];
            }
            next_to_visit.push(point);
        }
    }

    return -1;
}

int first_part_2023_17() {
    std::vector<std::vector<int>> grid;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(),
                  [&grid](std::string str) {
                      grid.emplace_back();
                      for (char c: str) {
                          grid.back().emplace_back(c - '0');
                      }
                  });
    std::cout << shortest_path_value<1, 3>(grid) << std::endl;
    return EXIT_SUCCESS;
}

int second_part_2023_17() {
    std::vector<std::vector<int>> grid;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(),
                  [&grid](std::string str) {
                      grid.emplace_back();
                      for (char c: str) {
                          grid.back().emplace_back(c - '0');
                      }
                  });
    std::cout << shortest_path_value<4, 10>(grid) << std::endl;
    return EXIT_SUCCESS;
}

#endif
