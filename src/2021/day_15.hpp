#ifndef AOC_2021_15
#define AOC_2021_15

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
#include <numeric>
#include <cmath>
#include <sstream>
#include "../input_source.hpp"
#include "../tools/word_delimited_by.h"
#include "../tools/std_extentions.hpp"

void update_neighbors(const auto &mat, auto &neighbors, auto &visited, int curr_i, int curr_j) {
    neighbors.clear();
    if (curr_j + 1 < mat[0].size()) {
        neighbors.push_back({curr_i, curr_j + 1});
    }
    if (curr_j - 1 >= 0) {
        neighbors.push_back({curr_i, curr_j - 1});
    }
    if (curr_i + 1 < mat.size()) {
        neighbors.push_back({curr_i + 1, curr_j});
    }
    if (curr_i - 1 >= 0) {
        neighbors.push_back({curr_i - 1, curr_j});
    }
}

void update_point_value(auto &mat, const auto &point, size_t curr_i, size_t curr_j) {
    auto new_potential_point_weight = mat[curr_i][curr_j].second + mat[point.first][point.second].first;
    if (new_potential_point_weight < mat[point.first][point.second].second) {
        mat[point.first][point.second].second = new_potential_point_weight;
    }
}

std::size_t shortest_path_value(auto &points) {
    std::vector<std::pair<size_t , size_t>> neighbors;
    std::set<std::pair<size_t , size_t>> next_to_visit;
    std::set<std::pair<size_t, size_t>> visited;
    points[0][0].second = 0;
    std::pair<size_t, size_t> current = {0, 0};
    next_to_visit.insert(current);
    while (!next_to_visit.empty()) {
        auto[curr_i, curr_j] = current;

        visited.insert(current);
        update_neighbors(points, neighbors, visited, curr_i, curr_j);
        for (auto &point: neighbors) {
            if (visited.find(point) != visited.end()) continue;
            update_point_value(points, point, curr_i, curr_j);
            next_to_visit.insert(point);
        }

        next_to_visit.erase(current);

        if (curr_i == points.size() - 1 && curr_j == points[0].size() - 1) break;

        size_t min_distance = -1;
        for (auto[x, y]: next_to_visit) {
            if (points[x][y].second < min_distance) {
                min_distance = points[x][y].second;
                current = std::pair<size_t, size_t>{x, y};
            }
        }
    }
    return points.at(points.size() - 1).at(points.at(points.size() - 1).size() - 1).second;
}

int first_part_2021_15() {
    std::vector<std::vector<std::pair<int, size_t>>> points;
    std::set<std::pair<int , int>> unvisited_points;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [&] (auto line) {
        points.template emplace_back();
        std::transform(line.begin(), line.end(), std::back_inserter(points.back()), [] (char number) {
            return std::pair{number - '0', -1};
        });
    });
    std::cout << shortest_path_value(points) << std::endl;

    return EXIT_SUCCESS;
}
/*void update_section_neighbors(const auto &mat, auto &neighbors, size_t curr_i, size_t curr_j, size_t section_i, size_t section_j) {
    neighbors.clear();
    if (curr_j + 1 < mat[0].size())
        neighbors.insert({curr_i, curr_j + 1, section_i, section_j});
    else neighbors.insert({curr_i, 0, section_i, section_j + 1});

    if (curr_i + 1 < mat.size())
        neighbors.insert({curr_i + 1, curr_j, section_i, section_j});
    else neighbors.insert({0, curr_j, section_i + 1, section_j});
}

void update_section_point_value(auto &mat, const auto &point, size_t curr_i, size_t curr_j) {
    auto current_point_val = mat[std::get<0>(point)][std::get<1>(point)].first + std::get<2>(point) + std::get<3>(point);
    current_point_val = current_point_val % 9 == 0 ? 9 : current_point_val;
    auto new_potential_point_weight = mat[curr_i][curr_j].second + current_point_val;
    if (new_potential_point_weight < mat[std::get<0>(point)][std::get<1>(point)].second) {
        mat[std::get<0>(point)][std::get<1>(point)].second = new_potential_point_weight;
    }
}

void reset_points_weight(auto &points, size_t start_i, size_t start_j) {
    for (; start_i < points.size(); start_i++) {
        for (; start_j < points[0].size(); start_j++) {
            points[start_i][start_j].second = -1;
        }
    }
}

std::size_t shortest_path_value_5x5(auto &points) {
    std::set<std::tuple<size_t, size_t, size_t, size_t>> neighbors;
    points[0][0].second = 0;
    for (size_t section_i = 0; section_i < 5; section_i++) {
        for (size_t section_j = 0; section_j < 5; section_j++) {
            for (size_t curr_i = 0; curr_i < points.size(); curr_i++) {
                for (size_t curr_j = 0; curr_j < points[0].size(); curr_j++) {
                    update_section_neighbors(points, neighbors, curr_i, curr_j, section_i, section_j);
                    for (auto &point: neighbors) {
                        update_section_point_value(points, point, curr_i, curr_j);
                    }
                }
            }
            if (section_j < 4) reset_points_weight(points, 0, 1);
            else reset_points_weight(points, 1, 0);
        }
    }
    return points.at(points.size() - 1).at(points.at(0).size() - 1).second;
}*/

int second_part_2021_15() {
    std::vector<std::vector<std::pair<size_t, size_t>>> points;
    std::set<std::pair<int , int>> unvisited_points;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [&] (auto line) {
        points.template emplace_back();
        std::transform(line.begin(), line.end(), std::back_inserter(points.back()), [] (char number) {
            return std::pair{number - '0', -1};
        });
    });

    auto original_height = points.size();
    auto original_width = points[0].size();
    points.resize(original_height * 5);
    for (auto &line : points) {
        line.resize(original_width * 5);
    }

    auto get_original_current_line = [&points, original_width] (size_t line) {
        auto res = points.at(line);
        res.resize(original_width);
        return res;
    };

    for (size_t section_i = 0; section_i < 5; section_i++) {
        for (size_t section_j = 0; section_j < 5; section_j++) {
            if (section_i == 0 && section_j == 0) continue;
            for (size_t line_i = 0; line_i < original_height; line_i++) {
                auto original_line = get_original_current_line(line_i);
                for (size_t curr_j = 0; curr_j < original_width; curr_j++) {

                    auto actual_line = line_i + original_height * section_i;
                    auto actual_j = curr_j + original_width * section_j;

                    points.at(actual_line).at(actual_j).first = (original_line[curr_j].first + section_i + section_j - 1) % 9 + 1;
                    points.at(actual_line).at(actual_j).second = -1;
                }
            }
        }
    }

   /* for (auto i = 0; i < points.size(); i++) {
        for (auto j = 0; j < points[i].size(); j++) {
            std::cout << points[i][j].first << " ";
        }
        std::cout << std::endl;
    }*/

    std::cout << shortest_path_value(points) << std::endl;

    return EXIT_SUCCESS;
}

#endif