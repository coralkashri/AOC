#ifndef AOC_2023_2
#define AOC_2023_2

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

namespace aoc_2023_day_2_tools {

    enum CubeColors {
        RED,
        GREEN,
        BLUE,

        COLORS_COUNT
    };
}

int first_part_2023_2() {
    const std::array<int, aoc_2023_day_2_tools::COLORS_COUNT> bag = {12, 13, 14};

    std::cout << std::accumulate(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE),
                                 std::istream_iterator<WordDelimitedBy<'\n'>>(),
                                 0ull, [id = 0ull, &bag](size_t res, std::string str) mutable -> size_t {
                ++id;
                size_t input_start = str.find(':');
                str_number current_number;
                for (size_t i = input_start + 2; i < str.size(); ++i) {
                    char c = str[i];

                    if (c == ' ') continue;

                    if (std::isdigit(c)) {
                        current_number += c;
                        continue;
                    }

                    aoc_2023_day_2_tools::CubeColors color;
                    switch (c) {
                        case 'r':
                            i += 4;
                            color = aoc_2023_day_2_tools::CubeColors::RED;
                            break;
                        case 'g':
                            i += 6;
                            color = aoc_2023_day_2_tools::CubeColors::GREEN;
                            break;
                        case 'b':
                            i += 5;
                            color = aoc_2023_day_2_tools::CubeColors::BLUE;
                            break;
                        default:
                            exit(EXIT_FAILURE);
                    }

                    if (current_number.get_number() > bag[color])
                        return res;

                    current_number.reset();
                }

                return res + id;
            }) << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2023_2() {
    std::array<long long, aoc_2023_day_2_tools::COLORS_COUNT> bag = {0, 0, 0};

    std::cout << std::accumulate(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE),
                                 std::istream_iterator<WordDelimitedBy<'\n'>>(),
                                 0ull, [id = 0ull, &bag](size_t res, std::string str) mutable -> size_t {
                ++id;
                size_t input_start = str.find(':');
                str_number current_number;
                for (size_t i = input_start + 2; i < str.size(); ++i) {
                    char c = str[i];

                    if (c == ' ') continue;

                    if (std::isdigit(c)) {
                        current_number += c;
                        continue;
                    }

                    aoc_2023_day_2_tools::CubeColors color;
                    switch (c) {
                        case 'r':
                            i += 4;
                            color = aoc_2023_day_2_tools::CubeColors::RED;
                            break;
                        case 'g':
                            i += 6;
                            color = aoc_2023_day_2_tools::CubeColors::GREEN;
                            break;
                        case 'b':
                            i += 5;
                            color = aoc_2023_day_2_tools::CubeColors::BLUE;
                            break;
                        default:
                            exit(EXIT_FAILURE);
                    }

                    bag[color] = std::max(bag[color], current_number.get_number());
                    current_number.reset();
                }

                res += bag[0] * bag[1] * bag[2];
                bag[0] = bag[1] = bag[2] = 0;
                return res;
            }) << std::endl;

    return EXIT_SUCCESS;
}

#endif