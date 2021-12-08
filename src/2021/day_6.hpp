#ifndef AOC_2021_6
#define AOC_2021_6

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include "../input_source.hpp"
#include <iostream>
#include <chrono>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>
#include <ranges>
#include <numeric>
#include "../input_source.hpp"

template<char delimiter>
class WordDelimitedBy : public std::string{};

template <char delimiter>
std::istream& operator>>(std::istream& is, WordDelimitedBy<delimiter>& output) {
    std::getline(is, output, delimiter);
    return is;
}

auto perform_new_day(std::vector<unsigned long long > &numbers) {
    std::rotate(numbers.begin(), numbers.begin() + 1, numbers.end());
    numbers.at(6) += numbers.back();
}

int first_part_2021_6() {
    // TODO Permutation
    std::vector<unsigned long long> fish_counter_by_number(9, 0);
    std::for_each(std::istream_iterator<WordDelimitedBy<','>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<','>>(), [&] (std::string str) {
        return fish_counter_by_number.at(std::stoi(str))++;
    });
    for (size_t i = 0; i < 256; i++) {
        perform_new_day(fish_counter_by_number);
    }
    std::cout << "size: " << std::accumulate(fish_counter_by_number.begin(), fish_counter_by_number.end(), 0ul) << std::endl;

    return EXIT_SUCCESS;
}

#endif
