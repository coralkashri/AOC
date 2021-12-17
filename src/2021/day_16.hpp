#ifndef AOC_2021_16
#define AOC_2021_16

#include <iostream>
#include <iterator>
#include <ranges>
#include <vector>
#include <bitset>
#include <list>
#include <chrono>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <sstream>
#include "../input_source.hpp"
#include "../tools/word_delimited_by.h"
#include "../tools/std_extentions.hpp"

using namespace std::string_literals;

template <int SectionSize>
size_t parse_section(std::string &str) {
    std::bitset<SectionSize> section_data(str.substr(0, SectionSize));
    str = str.substr(SectionSize);
    return section_data.to_ulong();
}

size_t parse_version(std::string &str) {
    return parse_section<3>(str);
}

size_t parse_id(std::string &str) {
    return parse_section<3>(str);
}

size_t parse_i(std::string &str) {
    return parse_section<1>(str);
}

long long parse_long_l(std::string &str) {
    return parse_section<15>(str);
}

long long parse_short_l(std::string &str) {
    return parse_section<11>(str);
}

size_t bin_to_dec(std::string s){
    size_t res = 0;
    for (size_t i = 0; i < s.size(); ++i) {
        res = (res | (unsigned int)(s[i] - '0')) << 1;
    }
    res >>= 1;
    return res;
}

size_t parse_number(std::string &str) {
    std::string number;
    auto get_current_bits = [&] () {
        str = str.substr(1);
        number += str.substr(0, 4);
        str = str.substr(4);
    };
    while (str.front() == '1') {
        get_current_bits();
    }
    get_current_bits();
    return bin_to_dec(number);
}

size_t parse_packet(std::string &str);

std::vector<size_t> get_all_numbers(std::string &str) {
    std::vector<size_t> res;
    long long sub_packets_size;
    std::string next_sub_packets;
    if (parse_i(str) == 0) { // 15
        sub_packets_size = parse_long_l(str);
        next_sub_packets = str.substr(0, sub_packets_size);
        str = str.substr(sub_packets_size);
        while (!next_sub_packets.empty()) {
            res.emplace_back(parse_packet(next_sub_packets));
        }
    } else { // 11
        sub_packets_size = parse_short_l(str);
        while (sub_packets_size--) {
            res.emplace_back(parse_packet(str));
        }
    }
    return res;
}

size_t action_on_id(size_t id, std::string &str) {
    std::vector<size_t> numbers;

    if (id != 4) numbers = get_all_numbers(str);

    switch (id) {
        case 4:
            return parse_number(str);

        case 0:
            return std::reduce(numbers.begin(), numbers.end());

        case 1:
            return std::reduce(numbers.begin(), numbers.end(), 1ull, std::multiplies<>{});

        case 2:
            return *std::min_element(numbers.begin(), numbers.end());

        case 3:
            return *std::max_element(numbers.begin(), numbers.end());

        case 5:
            return numbers.front() > numbers.back();

        case 6:
            return numbers.front() < numbers.back();

        case 7:
            return numbers.front() == numbers.back();

        default:
            throw std::runtime_error("Unexpected type id.");
    }
}

size_t parse_packet(std::string &str) {
    auto version = parse_version(str);
    auto id = parse_id(str);
    return action_on_id(id, str);
}

int first_part_2021_16() {
    std::map<char, std::string> const hex_values = {
        {'0', "0000"}, {'1', "0001"}, {'2', "0010"},
        {'3', "0011"}, {'4', "0100"}, {'5', "0101"},
        {'6', "0110"}, {'7', "0111"}, {'8', "1000"},
        {'9', "1001"}, {'A', "1010"}, {'B', "1011"},
        {'C', "1100"}, {'D', "1101"}, {'E', "1110"},
        {'F', "1111"}
    };
    std::string code;
    std::for_each(std::istream_iterator<char>(INPUT_SOURCE), std::istream_iterator<char>(), [&] (char c) {
        code += hex_values.at(c);
    });
    std::cout << parse_packet(code);


    return EXIT_SUCCESS;
}

#endif