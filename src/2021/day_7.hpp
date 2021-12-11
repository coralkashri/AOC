#ifndef AOC_2021_7
#define AOC_2021_7

#include <iostream>
#include <iterator>
#include <vector>
#include <list>
#include <chrono>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>
#include <ranges>
#include <numeric>
#include <cmath>
#include "../input_source.hpp"
#include "../tools/word_delimited_by.h"

int first_part_2021_7() {
    std::vector<size_t> crab_locations;
    std::transform(std::istream_iterator<WordDelimitedBy<','>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<','>>(), std::inserter(crab_locations, crab_locations.end()), [&] (auto str) {
        return std::stoll(str);
    });
    std::sort(crab_locations.begin(), crab_locations.end());
    auto target_location = *std::next(crab_locations.begin(), crab_locations.size() / 2);

    std::cout << "Required fuel: " << std::transform_reduce(crab_locations.begin(), crab_locations.end(), 0, std::plus<>{}, [target_location] (auto number) {
        return std::abs((long long)(target_location - number));
    }) << std::endl;

    return EXIT_SUCCESS;
}

size_t calculate_required_fuel(size_t distance){
    return distance * (distance + 1) / 2;
}

int second_part_2021_7() {
    std::vector<size_t> crab_locations;
    std::transform(std::istream_iterator<WordDelimitedBy<','>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<','>>(), std::inserter(crab_locations, crab_locations.end()), [&] (auto str) {
        return std::stoll(str);
    });
    auto [brute_force_start, brute_force_stop] = std::minmax_element(crab_locations.begin(), crab_locations.end());
    size_t minimum_cost = -1;
    for (auto current_location_test = *brute_force_start; current_location_test < *brute_force_stop; current_location_test++) {
        size_t num = 0;
        std::for_each(crab_locations.begin(), crab_locations.end(), [current_location_test, &num] (auto actual_crab_location) {
            num += calculate_required_fuel(std::abs((long long)(actual_crab_location - current_location_test)));
        });
        minimum_cost = std::min(minimum_cost, num);
    }

    std::cout << "Required fuel: " << minimum_cost << std::endl;

    return EXIT_SUCCESS;
}

#endif