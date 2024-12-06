#ifndef ADVENTOFCODE2024_DAY_1_HPP
#define ADVENTOFCODE2024_DAY_1_HPP

#include <iostream>
#include <algorithm>
#include <iterator>
#include <map>
#include <ranges>
#include <numeric>
#include <regex>

#include "../input_source.hpp"
#include "../tools/str_number.hpp"
#include "../tools/word_delimited_by.h"

int first_part_2024_3() {
    std::cout << std::transform_reduce(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), 0ull, std::plus{}, [](std::string str) {
        size_t sum = 0;

        std::regex word_regex(R"(mul\([\d]{1,3},[\d]{1,3}\))");
        for (auto it = std::sregex_iterator(str.begin(), str.end(), word_regex); it != std::sregex_iterator(); ++it) {
            std::string mstr = it->str();
            std::string_view vstr = mstr;
            size_t comma_location = vstr.find(',');
            str_number<int> n1 = vstr.substr(4, comma_location - 4);
            str_number<int> n2 = vstr.substr(comma_location + 1, vstr.rfind(')') - comma_location - 1);
            sum += n1.get_number() * n2.get_number();
        }

        return sum;
    }) << "\n";
    return EXIT_SUCCESS;
}

int second_part_2024_3() {
    std::cout << std::transform_reduce(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), 0ull, std::plus{}, [enabled = true](std::string str) mutable {
        size_t sum = 0;

        std::regex word_regex(R"(mul\([\d]{1,3},[\d]{1,3}\)|don't|do)");
        for (auto it = std::sregex_iterator(str.begin(), str.end(), word_regex); it != std::sregex_iterator(); ++it) {
            std::string mstr = it->str();

            if (mstr == "do") {
                enabled = true;
                continue;
            } else if (mstr == "don't") {
                enabled = false;
                continue;
            }
            if (!enabled) continue;

            std::string_view vstr = mstr;
            size_t comma_location = vstr.find(',');
            str_number<int> n1 = vstr.substr(4, comma_location - 4);
            str_number<int> n2 = vstr.substr(comma_location + 1, vstr.rfind(')') - comma_location - 1);
            sum += n1.get_number() * n2.get_number();
        }

        return sum;
    }) << "\n";
    return EXIT_SUCCESS;
}

#endif //ADVENTOFCODE2024_DAY_1_HPP
