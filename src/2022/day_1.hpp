#ifndef AOC_2022_1
#define AOC_2022_1

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include "../input_source.hpp"

int first_part_2022_1() {
    size_t max = 0;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&max, current = 0ll] (std::string str) mutable {
        if (str.empty()) {
            current = 0ll;
            return;
        }
        auto current_number = std::stol(str);
        current += current_number;
        if (current > max) {
            max = current;
        }
    });
    
    std::cout << max << "\n";

    return EXIT_SUCCESS;
}

int second_part_2022_1() {
    std::set<size_t> maxes;
    size_t current = 0ll;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&maxes, &current] (std::string str) mutable {
        if (str.empty()) {
            maxes.insert(current);
            current = 0ll;
            return;
        }
        auto current_number = std::stol(str);
        current += current_number;
    });
    
    maxes.insert(current);
    
    std::cout << std::accumulate(maxes.rbegin(), std::next(maxes.rbegin(), 3), 0) << "\n";

    return EXIT_SUCCESS;
}

#endif