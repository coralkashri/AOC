#ifndef AOC_2023_23
#define AOC_2023_23

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
    point_xd<2> location;
    std::pair<int, int> direction;
    int64_t heatLoss = 0;
    int64_t y_parent, x_parent;
    std::set<point_xd<2>> visited;
    //std::vector<std::pair<int, int>> visited_directions;

    bool operator<(const next_point &ref) const {
        return heatLoss < ref.heatLoss;
    }
};

void trace_path(const auto &mat, const std::vector<next_point>& points) {
    std::stack<next_point> path;
    auto current = points.back();

    while (current.location[0] != 0 || current.location[1] != 0) {
        path.push(current);
        current = *std::find_if(points.rbegin(), points.rend(), [yp = current.y_parent, xp = current.x_parent](const next_point& p) {
            return p.location[0] == yp && p.location[1] == xp;
        });
    }

    while (!path.empty()) {
        current = path.top();
        path.pop();
        std::cout << "(" << current.location[0] << ", " << current.location[1] << " [" << current.heatLoss << "]" ")" << " -> ";
    }

    std::cout << std::endl;
}

void update_neighbors(const auto &mat, auto &neighbors, const next_point &current) {
    std::vector<point_xd<2>> possible_directions = {{0,  1},
                                                            {0,  -1},
                                                            {1,  0},
                                                            {-1, 0}};
    neighbors.clear();

    for (auto &d: possible_directions) {
        bool is_opposite_direction = d[0] == -current.direction.first && d[1] == -current.direction.second;
        if (is_opposite_direction) continue;
        if (current.location[0] + d[0] < 0 || current.location[0] + d[0] >= mat.size()) continue;
        if (current.location[1] + d[1] < 0 || current.location[1] + d[1] >= mat[0].size()) continue;
        point_xd<2> new_location = current.location + d;

        switch (mat[new_location[0]][new_location[1]]) {
            case '>':
                d += {0, 1};
                break;
            case 'v':
                d += {1, 0};
                break;
            case '^':
                d += {-1, 0};
                break;
            case '<':
                d += {0, -1};
                break;
            case '#':
                d = {-9, -9};
                break;
            case '.':
                break;
            default:
                exit(1);
        }

        if (d == point_xd<2>{-9, -9}) continue;
        if (d == point_xd<2>{0, 0}) continue;

        new_location = current.location + d;

        neighbors.push_back(next_point{.location = new_location, .direction = {d[0], d[1]}});
    }
}

std::size_t longest_path_value(auto &points) {
    std::vector<next_point> neighbors;
    std::priority_queue<next_point, std::vector<next_point>, std::less<>> next_to_visit;
    next_point current = {.location = {0, 1}, .direction = {0, 0}, .heatLoss = 0};
    next_to_visit.push(current);
    int64_t max_steps = 0;
    while (!next_to_visit.empty()) {
        current = next_to_visit.top();
        next_to_visit.pop();

        if (points[current.location[0]][current.location[1]] == '#') continue;

        if (current.visited.contains(current.location)) continue;
        current.visited.insert(current.location);

        if (current.location[0] == points.size() - 1 && current.location[1] == points[0].size() - 2) {
            max_steps = std::max(max_steps, current.heatLoss);
            std::cout << max_steps << std::endl;
            continue;
        }

        update_neighbors(points, neighbors, current);

        for (auto &point: neighbors) {
            point.heatLoss = current.heatLoss;
            int steps_count = std::abs(point.direction.first + point.direction.second);
            point.heatLoss += steps_count;
            point.visited = current.visited;
            next_to_visit.push(point);
        }
    }

    return max_steps;
}

int first_part_2023_23() {
    std::vector<std::string> grid;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(),
                  [&grid](std::string str) {
                      grid.push_back(str);
                  });

    std::cout << longest_path_value(grid) << std::endl;

    return EXIT_SUCCESS;
}

void update_neighbors(const auto &mat, auto &neighbors, auto &visited, int curr_i, int curr_j) {
    neighbors.clear();
    if (curr_j + 1 < mat[0].size() && mat[curr_i][curr_j + 1].first != '#') {
        neighbors.push_back({curr_i, curr_j + 1});
    }
    if (curr_j - 1 >= 0 && mat[curr_i][curr_j - 1].first != '#') {
        neighbors.push_back({curr_i, curr_j - 1});
    }
    if (curr_i + 1 < mat.size() && mat[curr_i + 1][curr_j].first != '#') {
        neighbors.push_back({curr_i + 1, curr_j});
    }
    if (curr_i - 1 >= 0 && mat[curr_i - 1][curr_j].first != '#') {
        neighbors.push_back({curr_i - 1, curr_j});
    }
}

std::size_t longest_path_value_2(auto &points) {
    std::vector<std::pair<size_t , size_t>> neighbors;
    std::stack<std::pair<size_t , size_t>> next_to_visit;
    std::set<std::pair<size_t, size_t>> visited;
    points[0][0].second = 0;
    std::pair<size_t, size_t> current = {0, 1};
    next_to_visit.push(current);
    while (!next_to_visit.empty()) {
        auto[curr_i, curr_j] = next_to_visit.top();
        next_to_visit.pop();
        visited.insert(current);

        update_neighbors(points, neighbors, visited, curr_i, curr_j);
        for (auto &point: neighbors) {
            if (visited.contains(point)) continue;
            points[point.first][point.second].second = std::max(points[point.first][point.second].second, points[curr_i][curr_j].second + 1);
            next_to_visit.push(point);
        }

        if (curr_i == points.size() - 1 && curr_j == points[0].size() - 2) continue;

        /*size_t max_distance = 0;
        for (auto[x, y]: next_to_visit) {
            if (points[x][y].second > max_distance) {
                max_distance = points[x][y].second;
                current = std::pair<size_t, size_t>{x, y};
            }
        }*/
    }
    return points[points.size() - 1][points[0].size() - 2].second;
}

int second_part_2023_23() {
    std::vector<std::vector<std::pair<char, size_t>>> grid;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(),
                  [&grid](std::string str) {
                      grid.emplace_back();
                      std::transform(str.begin(), str.end(), std::back_inserter(grid.back()), [] (char c) {
                          return std::pair{c, 0};
                      });
                  });

    std::cout << longest_path_value_2(grid) << std::endl;

    return EXIT_SUCCESS;
}

#endif
