#ifndef AOC_2021_14
#define AOC_2021_14

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
#include <sstream>
#include "../input_source.hpp"
#include "../tools/word_delimited_by.h"
#include "../tools/std_extentions.hpp"

using namespace std::string_literals;

char get_char_from_pair(const std::map<std::string, char> &pair_insertions, std::string pair_chars) {
    return pair_insertions.at(pair_chars);
}

void perform_modification(const std::map<std::string, char> &pair_insertions, std::string &res) {
    std::string res_temp = res;
    size_t res_temp_i = 1;
    for (auto res_it = res.begin() + 1; res_it != res.end(); res_it++) {
        auto insertion_char = get_char_from_pair(pair_insertions, ""s + *(res_it - 1) + *res_it);
        res_temp.insert(res_temp_i, 1, insertion_char);
        res_temp_i += 2;
    }
    res = res_temp;
}

int first_part_2021_14() {
    std::map<std::string, char> pair_insertions;
    std::string result;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (std::string line) {
        if (line.empty()) return;
        size_t sep = line.find("->");
        if (sep == std::string::npos) result = line;
        else pair_insertions[line.substr(0, sep - 1)] = line.substr(sep + 3).at(0);
    });

    std::set<char> exist_chars;
    std::map<size_t, char> letters_count;

    for (size_t i = 0; i < 10; i++)
        perform_modification(pair_insertions, result);

    exist_chars.insert(result.begin(), result.end());
    for (auto e_char : exist_chars) {
        letters_count[std::count(result.begin(), result.end(), e_char)] = e_char;
    }

    std::cout << (*letters_count.rbegin()).first - (*letters_count.begin()).first << std::endl;


    return EXIT_SUCCESS;
}

void update_existing_pairs(std::map<std::string, size_t> &existing_pairs, const std::map<std::string, size_t> &new_pairs) {
    for (auto &n_pair : new_pairs) {
        existing_pairs[n_pair.first] += n_pair.second;
    }
}

void perform_counting(const std::map<std::string, char> &pair_insertions, std::map<std::string, size_t> &existing_pairs) {
    std::map<std::string, size_t> new_pairs;
    for (const auto &e_pair : existing_pairs) {
        auto inserted_char = get_char_from_pair(pair_insertions, e_pair.first);
        new_pairs[e_pair.first.substr(0, 1) + inserted_char] += e_pair.second;
        new_pairs[inserted_char + e_pair.first.substr(1)] += e_pair.second;
    }
    existing_pairs = new_pairs;
}

std::map<std::string, size_t> get_existing_pairs_from_string(std::string str) {
    std::map<std::string, size_t> res;
    for (auto str_it = str.begin() + 1; str_it != str.end(); str_it++) {
        res[""s + *(str_it - 1) + *str_it]++;
    }
    return res;
}

int second_part_2021_14() {
    std::map<std::string, char> pair_insertions;
    std::string result;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (std::string line) {
        if (line.empty()) return;
        size_t sep = line.find("->");
        if (sep == std::string::npos) result = line;
        else pair_insertions[line.substr(0, sep - 1)] = line.substr(sep + 3).at(0);
    });
    auto existing_pairs = get_existing_pairs_from_string(result);

    std::map<char, size_t> letters_count;

    for (size_t i = 0; i < 40; i++)
        perform_counting(pair_insertions, existing_pairs);

    for (auto e_pair : existing_pairs) {
        letters_count[e_pair.first.substr(1).at(0)] += e_pair.second;
        letters_count[e_pair.first.substr(0, 1).at(0)] += e_pair.second;
    }

    size_t min_val, max_val;
    max_val = 0;
    min_val = -1;
    for (auto c : letters_count) {
        c.second = std::ceil(c.second / 2.);
        if (c.second < min_val) min_val = c.second;
        if (c.second > max_val) max_val = c.second;
    }

    std::cout << max_val - min_val << std::endl;

    return EXIT_SUCCESS;
}

#endif