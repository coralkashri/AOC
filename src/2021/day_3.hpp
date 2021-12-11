#ifndef AOC_2021_3
#define AOC_2021_3

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <cmath>
#include <list>
#include "../input_source.hpp"

auto constexpr bits_count = 12;

int first_part_2021_3() {
    std::vector<size_t> counter(bits_count, 0);
    auto counter_place = counter.data();
    size_t total_input_size = 0;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [&] (auto elem) {
        total_input_size++;
        for (auto current_bit : elem) *counter_place++ += current_bit - '0';
        counter_place = counter.data();
    });
    std::bitset<bits_count> bits;
    char current_bit = bits_count - 1;
    for (auto ones_count : counter)
        bits[current_bit--] = ones_count > total_input_size / 2;
    std::cout << bits.to_ullong() * (~bits).to_ullong() << std::endl;
    return EXIT_SUCCESS;
}

inline auto bit_in_place(const auto &num, char bit_place) {
    return (num >> (bits_count - bit_place - 1)) & 1;
}

auto find_most_common_bit_in_place(char place, auto start_it, auto stop_it) {
    size_t current_bit_place_counter = std::count_if(start_it, stop_it, [=] (const auto &elem) {
        return bit_in_place(elem, place);
    });

    return current_bit_place_counter >= std::ceil(std::distance(start_it, stop_it) / 2.);
}

auto common_bit_number_search(auto start_it, auto stop_it, bool is_most_common_bit) {
    char counter_place = 0;
    while (std::distance(start_it, stop_it) > 1) {
        auto common_bit = find_most_common_bit_in_place(counter_place, start_it, stop_it) == is_most_common_bit;
        stop_it = std::partition(start_it, stop_it, [counter_place, common_bit](const auto &number) {
            return bit_in_place(number, counter_place) == common_bit;
        });
        counter_place = (char)((counter_place + 1) % bits_count);
    }
    return *start_it;
}

int second_part_2021_3() {
    std::vector<size_t> all_bits_numbers;

    std::transform(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), std::back_inserter(all_bits_numbers), [] (std::string str) {
        std::bitset<bits_count> number(str);
        return number.to_ullong();
    });

    auto most_common_found_number = common_bit_number_search(all_bits_numbers.begin(), all_bits_numbers.end(), true);
    auto less_common_found_number = common_bit_number_search(all_bits_numbers.begin(), all_bits_numbers.end(), false);
    std::cout << most_common_found_number * less_common_found_number << std::endl;

    return EXIT_SUCCESS;
}

#endif