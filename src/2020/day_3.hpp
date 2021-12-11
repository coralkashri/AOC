#ifndef AOC_2020_3
#define AOC_2020_3

#include <iostream>
#include <unordered_map>
#include <set>
#include <map>
#include <algorithm>
#include <numeric>
#include "../input_source.hpp"
#include "../tools/word_delimited_by.h"

int first_part_2020_3() {
    size_t current_location = 0;
    std::cout << "Trees in the way: " << std::count_if(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (std::string str) {
        auto res = str.at(current_location) == '#';
        current_location += 3;
        current_location %= str.size();
        return res;
    }) << std::endl;
    return EXIT_SUCCESS;
}

int second_part_2020_3() {
    //                 right   down     r-location
    std::map<std::pair<size_t, size_t>, size_t> movements = {
            {{1, 1}, 0},
            {{3, 1}, 0},
            {{5, 1}, 0},
            {{7, 1}, 0},
            {{1, 2}, 0}
    };
    std::vector<size_t> trees_in_the_way(movements.size(), 0);
    size_t current_line = 0;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (std::string str) {
        size_t movement_idx = 0;
        for (auto &data : movements) {
            if (current_line % data.first.second == 0) {
                trees_in_the_way.at(movement_idx) += str.at(data.second) == '#';
                data.second += data.first.first;
                data.second %= str.size();
            }
            movement_idx++;
        }
        current_line++;
    });
    std::cout << "Trees in the way: " << std::reduce(trees_in_the_way.begin(), trees_in_the_way.end(), 1ull, std::multiplies<>{}) << std::endl;
    return EXIT_SUCCESS;
}

#endif