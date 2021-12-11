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
#include "../input_source.hpp"
#include "../tools/word_delimited_by.h"

int first_part_2021_12() {
    std::vector<std::vector<int>> mat;
    size_t res = 0;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [&] (auto line) {
        mat.template emplace_back();
        line = "-" + line + "-"; // Add padding
        std::transform(line.begin(), line.end(), std::back_inserter(mat.back()), [] (char number) {
            return number - '0';
        });
    });
    mat.insert(mat.begin(), std::vector<int>(mat.at(0).size(), -3)); // Add padding
    mat.emplace_back(mat.at(0).size(), -3); // Add padding

    std::cout << "Result: " << res << std::endl;

    return EXIT_SUCCESS;
}

#endif