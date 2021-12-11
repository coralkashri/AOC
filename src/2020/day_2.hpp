#ifndef AOC_2020_2
#define AOC_2020_2

#include <iostream>
#include <unordered_map>
#include <set>
#include <algorithm>
#include "../input_source.hpp"
#include "../tools/word_delimited_by.h"

int first_part_2020_2() {
    std::cout << "Legal passwords count: " << std::count_if(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [] (std::string str) {
        auto range_middle = std::find(str.begin(), str.end(), '-');
        auto range_end = std::find(str.begin(), str.end(), ':') - 2;
        auto min_range = std::stoi(str.substr(0, std::distance(str.begin(), range_middle)));
        auto max_range = std::stoi(str.substr(std::distance(str.begin(), range_middle + 1), std::distance(range_middle + 1, range_end)));
        char relevant_char = str.substr(std::distance(str.begin(), range_end + 1), 1).at(0);
        auto password_case = str.substr(std::distance(str.begin(), range_end + 4));

        auto relevant_char_count_in_password = std::count(password_case.begin(), password_case.end(), relevant_char);

        return relevant_char_count_in_password >= min_range && relevant_char_count_in_password <= max_range;
    }) << std::endl;
    return EXIT_SUCCESS;
}

int second_part_2020_2() {
    std::cout << "Legal passwords count: " << std::count_if(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [] (std::string str) {
        auto range_middle = std::find(str.begin(), str.end(), '-');
        auto range_end = std::find(str.begin(), str.end(), ':') - 2;
        auto first_location = std::stoi(str.substr(0, std::distance(str.begin(), range_middle))) - 1;
        auto second_location = std::stoi(str.substr(std::distance(str.begin(), range_middle + 1), std::distance(range_middle + 1, range_end))) - 1;
        char relevant_char = str.substr(std::distance(str.begin(), range_end + 1), 1).at(0);
        auto password_case = str.substr(std::distance(str.begin(), range_end + 4));

        return (password_case.at(first_location) == relevant_char && password_case.at(second_location) != relevant_char) ||
                (password_case.at(first_location) != relevant_char && password_case.at(second_location) == relevant_char);
    }) << std::endl;
    return EXIT_SUCCESS;
}

#endif