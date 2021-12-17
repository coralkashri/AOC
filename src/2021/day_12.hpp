#ifndef AOC_2021_12
#define AOC_2021_12

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

bool is_start(const std::string &str) {
    return str == "start";
}

bool is_end(const std::string &str) {
    return str == "end";
}

bool is_big_cave(char cave) {
    return cave >= 'A' && cave <= 'Z';
}

bool is_legal_path(std::string current_point, std::vector<std::string> prev_points) {
    if (is_big_cave(current_point.at(0)) || is_end(current_point)) return true;
    return std::find(prev_points.begin(), prev_points.end(), current_point) == prev_points.end(); // Not listed small cave
}

bool is_complete_path(const std::string &path) {
    std::stringstream s("");
    s.str(path);
    std::string current_point;
    while (std::getline(s, current_point, ',')) {}
    return is_end(current_point);
}

std::vector<std::string> get_available_paths(std::map<std::string, std::vector<std::string>> map, std::string current_point, std::vector<std::string> prev_small_caves) {
    std::vector<std::string> res;
    if (!is_legal_path(current_point, prev_small_caves)) return res;
    if (is_end(current_point)) {
        res.push_back(current_point);
        return res;
    }
    if (!is_big_cave(current_point.at(0))) prev_small_caves.push_back(current_point);
    auto current_options = map.at(current_point);
    for (const auto &option : current_options) {
        auto optional_results = get_available_paths(map, option, prev_small_caves);
        for (const auto &optional_res : optional_results) {
            if (is_complete_path(optional_res)) {
                res.emplace_back(current_point + "," + optional_res);
            }
        }
    }
    return res;
}

int first_part_2021_12() {
    std::map<std::string, std::vector<std::string>> paths;
    std::vector<std::string> legal_paths;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [&] (auto line) {
        auto first_section_end = std::distance(line.begin(), std::find(line.begin(), line.end(), '-'));
        std::string start = line.substr(0, first_section_end);
        std::string destination = line.substr(first_section_end + 1);
        if (is_start(start) || is_start(destination) || is_end(start) || is_end(destination)) {
            if (is_start(start)) paths[start].push_back(destination);
            else if (is_start(destination)) paths[destination].push_back(start);
            else if (is_end(start)) paths[destination].push_back(start);
            else paths[start].push_back(destination);
        } else {
            paths[start].push_back(destination);
            paths[destination].push_back(start);
        }
    });

    auto start_points = paths.at("start");

    for (const auto &start_point : start_points) {
        std::vector<std::string> prev_small_caves;
        auto available_legal_paths_for_point = get_available_paths(paths, start_point, prev_small_caves);
        legal_paths.insert(legal_paths.end(), available_legal_paths_for_point.begin(), available_legal_paths_for_point.end());
    }

    std::cout << "Result: " << legal_paths.size() << std::endl;

    return EXIT_SUCCESS;
}

bool is_there_small_cave_twice(std::vector<std::string> prev_points) {
    for (auto point : prev_points) {
        if (is_end(point) || is_big_cave(point.at(0))) continue;
        if (std::count_if(prev_points.begin(), prev_points.end(), [point] (auto small_cave) {
            return point == small_cave;
        }) >= 2) return true;
    }
    return false;
}

bool is_legal_path_2(std::string current_point, std::vector<std::string> prev_points) {
    if (is_big_cave(current_point.at(0)) || is_end(current_point) || std::find(prev_points.begin(), prev_points.end(), current_point) == prev_points.end()) return true;
    return !is_there_small_cave_twice(prev_points);
}

std::vector<std::string> get_available_paths_2(std::map<std::string, std::vector<std::string>> map, std::string current_point, std::vector<std::string> prev_small_caves) {
    std::vector<std::string> res;
    if (!is_legal_path_2(current_point, prev_small_caves)) return res;
    if (is_end(current_point)) {
        res.push_back(current_point);
        return res;
    }
    if (!is_big_cave(current_point.at(0))) prev_small_caves.push_back(current_point);
    auto current_options = map.at(current_point);
    for (const auto &option : current_options) {
        auto optional_results = get_available_paths_2(map, option, prev_small_caves);
        for (const auto &optional_res : optional_results) {
            if (is_complete_path(optional_res)) {
                res.emplace_back(current_point + "," + optional_res);
            }
        }
    }
    return res;
}

int second_part_2021_12() {
    std::map<std::string, std::vector<std::string>> paths;
    std::vector<std::string> legal_paths;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [&] (auto line) {
        auto first_section_end = std::distance(line.begin(), std::find(line.begin(), line.end(), '-'));
        std::string start = line.substr(0, first_section_end);
        std::string destination = line.substr(first_section_end + 1);
        if (is_start(start) || is_start(destination) || is_end(start) || is_end(destination)) {
            if (is_start(start)) paths[start].push_back(destination);
            else if (is_start(destination)) paths[destination].push_back(start);
            else if (is_end(start)) paths[destination].push_back(start);
            else paths[start].push_back(destination);
        } else {
            paths[start].push_back(destination);
            paths[destination].push_back(start);
        }
    });

    auto start_points = paths.at("start");

    for (const auto &start_point : start_points) {
        std::vector<std::string> prev_small_caves;
        auto available_legal_paths_for_point = get_available_paths_2(paths, start_point, prev_small_caves);
        legal_paths.insert(legal_paths.end(), available_legal_paths_for_point.begin(), available_legal_paths_for_point.end());
    }

    std::cout << "Second Result: " << legal_paths.size() << std::endl;

    return EXIT_SUCCESS;
}

#endif