#ifndef AOC_2023_1
#define AOC_2023_1

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

namespace aoc_2023_day_1_tools {

    class numbers_tracker {
    private:
        struct Number {
            std::string num_str;
            uint8_t ltr = 0;
            uint8_t rtl = 0;
            uint8_t actual_number;
        };

    public:
        numbers_tracker() : potential_numbers({
                                                      {.num_str = "one", .actual_number = 1},
                                                      {.num_str = "two", .actual_number = 2},
                                                      {.num_str = "three", .actual_number = 3},
                                                      {.num_str = "four", .actual_number = 4},
                                                      {.num_str = "five", .actual_number = 5},
                                                      {.num_str = "six", .actual_number = 6},
                                                      {.num_str = "seven", .actual_number = 7},
                                                      {.num_str = "eight", .actual_number = 8},
                                                      {.num_str = "nine", .actual_number = 9},
                                              }) {}

        uint8_t set_current_char_ltr(char c) {
            for (auto&[number, ltr, rtl, actual_number]: potential_numbers) {
                const size_t number_size = number.size();
                if (number[ltr] == c) {
                    if (++ltr == number_size)
                        return actual_number;
                } else ltr = number[0] == c;
            }

            return 10;
        }

        uint8_t set_current_char_rtl(char c) {
            for (auto&[number, ltr, rtl, actual_number]: potential_numbers) {
                const size_t number_size = number.size();
                if (number[number_size - rtl - 1] == c) {
                    if (++rtl == number_size)
                        return actual_number;
                } else rtl = number[number_size - 1] == c;
            }

            return 10;
        }

        void reset() {
            for (auto&[number, ltr, rtl, actual_number]: potential_numbers) {
                ltr = rtl = 0;
            }
        }

    private:
        std::vector<Number> potential_numbers;
    };
}

int first_part_2023_1() {
    std::cout << std::accumulate(std::istream_iterator<std::string>(INPUT_SOURCE),
                                 std::istream_iterator<std::string>(),
                                 0ull, [](size_t res, const std::string &str) mutable {
                int first = 10, second = 10;
                /*// Less efficient, more elegant way:
                for (long i = 0, j = ssize(str) - 1; i < str.size(); ++i, --j) {
                     if (first == 10 && std::isdigit(str[i])) {
                         first = str[i] - '0';
                     }

                     if (second == 10 && std::isdigit(str[j])) {
                         second = str[j] - '0';
                     }
                }*/

                for (char c : str) {
                    if (std::isdigit(c)) {
                        first = c - '0';
                        break;
                    }
                }

                for (long i = ssize(str) - 1; i >= 0; i--) {
                    if (std::isdigit(str[i])) {
                        second = str[i] - '0';
                        break;
                    }
                }
                return res + first * 10 + second;
            }) << std::endl;
    return EXIT_SUCCESS;
}

int second_part_2023_1() {
    std::cout << std::accumulate(std::istream_iterator<std::string>(INPUT_SOURCE),
                                 std::istream_iterator<std::string>(),
                                 0ull, [nt = aoc_2023_day_1_tools::numbers_tracker()](size_t res, const std::string &str) mutable {
                int first = 10, second = 10;

                for (char c : str) {
                    if (std::isdigit(c)) {
                        first = c - '0';
                        break;
                    }

                    if (auto n = nt.set_current_char_ltr(c); n != 10) {
                        first = n;
                        break;
                    }
                }

                for (long i = ssize(str) - 1; i >= 0; i--) {
                    if (std::isdigit(str[i])) {
                        second = str[i] - '0';
                        break;
                    }

                    if (auto n = nt.set_current_char_rtl(str[i]); n != 10) {
                        second = n;
                        break;
                    }
                }

                nt.reset();

                return res + first * 10 + second;
            }) << std::endl;
    return EXIT_SUCCESS;
}

#endif