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

struct graph_point {
    point_xd<2> location{};
    std::vector<std::pair<graph_point*, size_t>> neighbors;
};

using graph = std::vector<graph_point>;

struct travel {
    std::vector<const graph_point*> locations;
    size_t distance;
};

std::vector<point_xd<2>> update_neighbors(const auto &mat, const point_xd<2>& current) {
    std::vector<point_xd<2>> possible_directions = {{0,  1},
                                                    {0,  -1},
                                                    {1,  0},
                                                    {-1, 0}};

    std::vector<point_xd<2>> locations;

    for (auto &d: possible_directions) {
        if (current[0] + d[0] < 0 || current[0] + d[0] >= mat.size()) continue;
        if (current[1] + d[1] < 0 || current[1] + d[1] >= mat[0].size()) continue;
        point_xd<2> new_location = current + d;

        if (mat[new_location[0]][new_location[1]] == '#') continue;

        locations.push_back(new_location);
    }

    return locations;
}

bool is_neighbor_exist(const std::vector<std::pair<graph_point*, size_t>>& neighbors, const graph_point* n) {
    return std::find_if(neighbors.begin(), neighbors.end(),
                        [&n](const std::pair<graph_point *, size_t> &p) {
                            return p.first->location == n->location;
                        }) != neighbors.end();
}

void find_neighbors(const std::vector<std::string>& grid, std::unordered_map<point_xd<2>, graph_point*>& points_map, graph_point* source) {
    std::set<point_xd<2>> visited;
    std::queue<std::pair<point_xd<2>, size_t>> travelers;
    travelers.push({source->location, 0});

    while (!travelers.empty()) {
        auto [p, distance] = travelers.front();
        travelers.pop();

        graph_point* current_point = nullptr;
        if (points_map.contains(p)) current_point = points_map.at(p);

        if (visited.contains(p)) continue;
        visited.insert(p);

        if (current_point && source != current_point) {
            if (!is_neighbor_exist(source->neighbors, current_point)) {
                source->neighbors.emplace_back(current_point, distance);
                current_point->neighbors.emplace_back(source, distance);
            }
            continue;
        }

        auto neighbors = update_neighbors(grid, p);

        for (const auto &n: neighbors) {
            travelers.push({n, distance + 1});
        }
    }
}

graph create_graph(const std::vector<std::string>& grid) {
    graph g;
    std::unordered_map<point_xd<2>, graph_point*> points_map;
    g.reserve(1000);

    g.push_back({0, 1});

    for (int64_t i = 0; i < grid.size(); ++i) {
        for (int64_t j = 0; j < grid[0].size(); ++j) {
            if (grid[i][j] != '#') {
                auto neighbors = update_neighbors(grid, {i, j});
                if (neighbors.size() > 2) {
                    g.push_back({.location = {i, j}, .neighbors = {}});
                    points_map[{i, j}] = &g.back();
                }
            }
        }
    }

    g.push_back({.location = {ssize(grid) - 1, ssize(grid[0]) - 2}, .neighbors = {}});
    points_map[{ssize(grid) - 1, ssize(grid[0]) - 2}] = &g.back();

    for (auto& p : g) {
        find_neighbors(grid, points_map, &p);
    }

    return g;
}

size_t longest_path_value_2(const graph& g) {
    std::stack<travel> travelers;
    travelers.push({.locations = {&g[0]}});
    size_t max_path = 0;

    while (!travelers.empty()) {
        travel traveler = travelers.top();
        travelers.pop();

        const graph_point* current_point = traveler.locations.back();

        if (current_point == &g.back()) {
            max_path = std::max(max_path, traveler.distance);
            continue;
        }

        std::set<const graph_point*> visited(traveler.locations.begin(),  traveler.locations.end());
        for (auto n : current_point->neighbors) {
            if (visited.contains(n.first)) continue;

            auto new_path = traveler.locations;
            new_path.push_back(n.first);
            travelers.push({.locations = new_path, .distance = traveler.distance + n.second});
        }
    }

    return max_path;
}

int second_part_2023_23() {
    std::vector<std::string> grid;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(),
                  [&grid](std::string str) {
                      grid.push_back(str);
                  });

    std::cout << longest_path_value_2(create_graph(grid)) << std::endl;

    return EXIT_SUCCESS;
}

#endif
