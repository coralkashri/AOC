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

int fact(int n){
    return (n==0) ? 0 : n + fact(n-1);
}

int first_part_2021_7() {
    std::vector<size_t> all_numbers_sorted;
    std::vector<size_t> sum_of_subs;
    std::set<size_t> res;
    std::transform(std::istream_iterator<WordDelimitedBy<','>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<','>>(), std::inserter(all_numbers_sorted, all_numbers_sorted.end()), [&] (auto str) {
        return std::stoll(str);
    });
    auto [minimum, max] = std::minmax_element(all_numbers_sorted.begin(), all_numbers_sorted.end());
    size_t minimum_cost;
    for (auto min = *minimum; min < *max; min++) {
        size_t num = 0;
        std::for_each(all_numbers_sorted.begin(), all_numbers_sorted.end(), [min, &num] (auto current_num) {
            num += current_num > min ? fact(current_num - min) : fact(min - current_num);
        });
        minimum_cost = std::min(minimum_cost, num);
    }

    std::cout << "size: " << minimum_cost << std::endl;

    return EXIT_SUCCESS;
}

#endif
