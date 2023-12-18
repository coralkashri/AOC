#ifndef AOC_2023_18
#define AOC_2023_18

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

size_t count_inside_cubics(std::vector<std::pair<int64_t, int64_t>>& points) {
    double area = 0.0;
    for(size_t i = 0; i < points.size(); ++i) {
        auto p0_x = points[i].second;
        auto p0_y = points[i].first;
        auto p1_x = points[(i + 1) % points.size()].second;
        auto p1_y = points[(i + 1) % points.size()].first;
        area += (double)p0_x * (double)p1_y - (double)p1_x * (double)p0_y;
    }
    return static_cast<size_t>(std::abs(area) / 2.);
}

template <bool UseHexadecimal>
std::pair<char, int64_t> get_direction_and_steps(const std::string& str) {
    if constexpr(!UseHexadecimal) {
        std::string_view str_view = str;
        char direction = str_view[0];
        str_number<int64_t> steps = str_view.substr(2, str_view.rfind(' ') - 2);
        return {direction, steps.get_number()};
    } else {
        char direction = str[str.size() - 2];
        if (direction == '0') direction = 'R';
        else if (direction == '1') direction = 'D';
        else if (direction == '2') direction = 'L';
        else if (direction == '3') direction = 'U';

        int64_t steps;
        from_hex(str.substr(str.size() - 7, 5), steps);
        return {direction, steps};
    }
}

void insert_point(std::vector<std::pair<int64_t, int64_t>> &points, int64_t& y, int64_t& x,  char direction, int64_t steps) {
    switch (direction) {
        case 'R':
            points.emplace_back(y, x += steps);
            break;
        case 'L':
            points.emplace_back(y, x -= steps);
            break;
        case 'U':
            points.emplace_back(y -= steps, x);
            break;
        case 'D':
        default:
            points.emplace_back(y += steps, x);
            break;
    }
}

template <bool UseHexadecimal>
size_t solve() {
    std::vector<std::pair<int64_t, int64_t>> points;
    size_t boundries_count = 0;
    points.emplace_back(0, 0);

    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE),
                  std::istream_iterator<WordDelimitedBy<'\n'>>(),
                  [&points, &boundries_count, y = 0z, x = 0z](std::string str) mutable {
                      auto [direction, steps] = get_direction_and_steps<UseHexadecimal>(str);
                      boundries_count += steps;
                      insert_point(points, y, x, direction, steps);
                  });
    return count_inside_cubics(points) + boundries_count / 2 + 1;
}

int first_part_2023_18() {
    std::cout << solve<false>() << std::endl;
    return EXIT_SUCCESS;
}

int second_part_2023_18() {
    std::cout << solve<true>() << std::endl;
    return EXIT_SUCCESS;
}

#endif
