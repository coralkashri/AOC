#ifndef AOC_2023_6
#define AOC_2023_6

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

template<typename StrNumbersT>
void solution() {
    std::vector<long long> times;
    std::vector<long long> distances;

    std::string input;
    std::getline(INPUT_SOURCE, input);
    times = StrNumbersT(input).get();
    std::getline(INPUT_SOURCE, input);
    distances = StrNumbersT(input).get();

    size_t mul = 1;

    for (size_t i = 0; i < times.size(); ++i) {
        size_t count = 0;

        for (long long hold_time = 0; hold_time <= times[i]; ++hold_time) {
            if ((times[i] - hold_time) * hold_time > distances[i]) count++;
            else if (count > 0) break;
        }

        mul *= count;
    }

    std::cout << mul << std::endl;
}

int first_part_2023_6() {
    solution<str_numbers<>>();

    return EXIT_SUCCESS;
}

int second_part_2023_6() {
    solution<str_numbers<long long, std::vector, '!'>>();

    return EXIT_SUCCESS;
}

#endif