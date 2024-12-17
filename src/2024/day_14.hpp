#ifndef ADVENTOFCODE2024_DAY_14_HPP
#define ADVENTOFCODE2024_DAY_14_HPP

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

template <bool ShouldPrintRobots>
class area {
public:
    area(long long seconds_to_predict) : seconds_to_predict(seconds_to_predict), picture(ShouldPrintRobots ? height : 0, std::vector<int>(width, 0)) {}

    void add_robot(point_xd<2> position, point_xd<2> velocity) {
        position[0] += velocity[0] * seconds_to_predict;
        position[1] += velocity[1] * seconds_to_predict;

        position[0] %= static_cast<long long>(width);
        position[1] %= static_cast<long long>(height);

        position[0] += position[0] >= 0 ? 0 : static_cast<long long>(width);
        position[1] += position[1] >= 0 ? 0 : static_cast<long long>(height);

        update_safe_factor(position);
    }

    size_t get_safety_factor() {
        size_t safety_factor = 1;

        if constexpr (ShouldPrintRobots) {
            for (size_t i = 0; i < height; ++i) {
                for (size_t j = 0; j < width; ++j) {
                    std::cout << ((picture[i][j] > 0) ? "*" : " ");
                }
                std::cout << "\n";
            }
        } else {
            for (auto f: safe_factors) {
                safety_factor *= f ? f : 1;
            }
        }

        return safety_factor;
    }

private:
    size_t width = USE_TEST_CASE ? 11 : 101;
    size_t height = USE_TEST_CASE ? 7 : 103;
    long long seconds_to_predict;
    size_t blind_column = width / 2;
    size_t blind_line = height / 2;
    std::array<size_t, 4> safe_factors{0};
    std::vector<std::vector<int>> picture;

    void update_safe_factor(point_xd<2> location) {
        if constexpr (ShouldPrintRobots) {
            picture[location[1]][location[0]]++;
        }

        if (location[0] == blind_column || location[1] == blind_line) return;

        if (location[0] < blind_column) {
            if (location[1] < blind_line) {
                // safe quadrant: 0
                ++safe_factors[0];
            } else {
                // safe quadrant: 2
                ++safe_factors[2];
            }
        } else {
            if (location[1] < blind_line) {
                // safe quadrant: 1
                ++safe_factors[1];
            } else {
                // safe quadrant: 3
                ++safe_factors[3];
            }
        }
    }
};

int first_part_2024_14() {
    area<false> a(100);
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&a](std::string str) {
        std::string_view strv = str;
        auto space_index = strv.find(' ');
        auto position_numbers = str_numbers<long long, std::vector, ','>(strv.substr(2, space_index - 2)).get();
        auto velocity_numbers = str_numbers<long long, std::vector, ','>(strv.substr(space_index + 3)).get();
        a.add_robot({position_numbers[0], position_numbers[1]}, {velocity_numbers[0], velocity_numbers[1]});
    });

    std::cout << a.get_safety_factor() << "\n";

    return EXIT_SUCCESS;
}

int second_part_2024_14() {
    size_t i = 50 + 103; // first point of group
    std::vector<std::string> input_lines;
    area<true> a(i);
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&a, &input_lines](std::string str) {
        input_lines.push_back(str);
        std::string_view strv = str;
        auto space_index = strv.find(' ');
        auto position_numbers = str_numbers<long long, std::vector, ','>(strv.substr(2, space_index - 2)).get();
        auto velocity_numbers = str_numbers<long long, std::vector, ','>(strv.substr(space_index + 3)).get();
        a.add_robot({position_numbers[0], position_numbers[1]}, {velocity_numbers[0], velocity_numbers[1]});
    });

    std::cout << a.get_safety_factor() << "\n";

    while (i < 10'000) {
        std::cout << (i += 103) << ": " << "\n";
        a = area<true>(i);
        for (std::string_view strv : input_lines) {
            auto space_index = strv.find(' ');
            auto position_numbers = str_numbers<long long, std::vector, ','>(strv.substr(2, space_index - 2)).get();
            auto velocity_numbers = str_numbers<long long, std::vector, ','>(strv.substr(space_index + 3)).get();
            a.add_robot({position_numbers[0], position_numbers[1]}, {velocity_numbers[0], velocity_numbers[1]});
        }

        std::cout << a.get_safety_factor() << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds (500));
    }

    return EXIT_SUCCESS;
}

#endif
