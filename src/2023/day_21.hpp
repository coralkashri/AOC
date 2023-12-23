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

std::vector<point_xd<2>> mark_near_locations(auto& points, const point_xd<2>& current_location) {
    static std::vector<point_xd<2>> relative_locations = {
            {0, 1},
            {0, -1},
            {1, 0},
            {-1, 0},
    };

    std::vector<point_xd<2>> new_locations;
    new_locations.reserve(4);

    points[current_location] = false;

    for (const auto& rel : relative_locations) {
        auto loc = current_location + rel;
        if (points.contains(loc)) {
            points[loc] = true;
            new_locations.push_back(loc);
        }
    }

    return new_locations;
}

std::vector<point_xd<2>> mark_near_locations_infinite_map(auto& points, size_t width, size_t height, const point_xd<2>& current_location) {
    static std::vector<point_xd<2>> relative_locations = {
            {0, 1},
            {0, -1},
            {1, 0},
            {-1, 0},
    };

    std::vector<point_xd<2>> new_locations;
    new_locations.reserve(4);

    points[current_location] = false;

    for (const auto& rel : relative_locations) {
        auto loc = current_location + rel;
        if (points.contains({std::abs(loc[0]) % (int64_t)height, std::abs(loc[1]) % (int64_t)width})) {
            points[loc] = true;
            new_locations.push_back(loc);
        }
    }

    return new_locations;
}

void perform_step(auto& points, auto& current_locations) {
    std::remove_reference_t<decltype(current_locations)> new_current_locations;

    for (const auto& loc : current_locations) {
        auto res = mark_near_locations(points, loc);
        new_current_locations.insert(res.begin(),  res.end());
    }

    current_locations.swap(new_current_locations);
}

void perform_step_infinite_map(auto& points, size_t width, size_t height, auto& current_locations) {
    std::remove_reference_t<decltype(current_locations)> new_current_locations;

    for (const auto& loc : current_locations) {
        auto res = mark_near_locations_infinite_map(points, width, height, loc);
        new_current_locations.insert(res.begin(),  res.end());
    }

    current_locations.swap(new_current_locations);
}

int first_part_2023_21() {
    std::unordered_map<point_xd<2>, bool> points;
    std::set<point_xd<2>> current_locations;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE),
                  std::istream_iterator<std::string>(),
                          [&points, &current_locations, y = 0ll] (std::string str) mutable {
        for (int64_t x = 0; x < ssize(str); ++x) {
            if (str[x] == '.') {
                points[{y, x}] = false;
            } else if (str[x] == 'S') {
                points[{y, x}] = true;
                current_locations.insert({y, x});
            }
        }
        ++y;
    });

    for (size_t i = 0; i < 64; ++i) {
        perform_step(points, current_locations);
    }

    std::cout << current_locations.size() << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2023_21() {
    /* TODO:
     *  This section is incomplete due to math related solution - will be solved in the future.
     */
    /*
    std::unordered_map<point_xd<2>, bool> points;
    std::set<point_xd<2>> current_locations;
    size_t width, height;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE),
                  std::istream_iterator<std::string>(),
                  [&points, &width, &height, &current_locations, y = 0ll] (std::string str) mutable {
                      width = str.size();
                      for (int64_t x = 0; x < ssize(str); ++x) {
                          if (str[x] == '.') {
                              points[{y, x}] = false;
                          } else if (str[x] == 'S') {
                              points[{y, x}] = true;
                              current_locations.insert({y, x});
                          }
                      }
                      height = ++y;
                  });

    for (size_t i = 0; i < 26501365; ++i) {
        perform_step_infinite_map(points, width, height, current_locations);
    }

    std::cout << current_locations.size() << std::endl;*/

    return EXIT_SUCCESS;
}

#endif
