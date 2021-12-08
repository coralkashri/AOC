#ifndef AOC_2021_8
#define AOC_2021_8

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
#include <sstream>
#include "../input_source.hpp"

template<char delimiter>
class WordDelimitedBy : public std::string{};

template <char delimiter>
std::istream& operator>>(std::istream& is, WordDelimitedBy<delimiter>& output) {
    std::getline(is, output, delimiter);
    return is;
}

bool is_combination_exists(std::string comb, std::map<size_t, std::string> known_numbers) {
    return std::find_if(known_numbers.begin(), known_numbers.end(), [comb] (auto pair) {
        if (pair.second == comb) return true;
        return false;
    }) != known_numbers.end();
}

std::vector<std::string> find_number(auto &numbers, size_t number, std::map<size_t, std::string> known_numbers) {
    std::vector<std::string> res;
    if (auto it = known_numbers.find(number); it != known_numbers.end()) {
        res.push_back(it->second);
        return res;
    }
    std::for_each(numbers.begin(), numbers.end(), [&] (auto line) {
        if (is_combination_exists(line, known_numbers)) return;
        if (line.size() == 6 && (number == 0 || number == 6 || number == 9)) {
            res.push_back(line);
        }
        if (line.size() == 5 && (number == 2 || number == 3 || number == 5)) {
            res.push_back(line);
        }
        if (line.size() == 2 && number == 1) {
            res.push_back(line);
        }
        if (line.size() == 4 && number == 4) {
            res.push_back(line);
        }
        if (line.size() == 3 && number == 7) {
            res.push_back(line);
        }
        if (line.size() == 7 && number == 8) {
            res.push_back(line);
        }
    });
    return res;
}

size_t parse_combination(std::string combination, std::map<size_t, std::string> known_numbers) {
    return std::find_if(known_numbers.begin(), known_numbers.end(), [combination] (auto pair) {
        if (pair.second == combination) return true;
        return false;
    })->first;
}

char find_diff(std::string a, std::string b) {
    for (auto segment : b) {
        for (auto segment_a : a) {
            if (segment_a == segment) continue;
            else return segment_a;
        }
    }
    return '-';
}

size_t num_of_missing_segments(std::string a, std::string b) {
    size_t res = 0;
    for (auto segment : b) {
        bool is_found = false;
        for (auto segment_a : a) {
            if (segment_a == segment) {
                is_found = true;
                break;
            }
        }
        if (!is_found) res++;
    }
    return res;
}

bool is_all_segment_exists(std::string a, std::string b) {
    return num_of_missing_segments(a, b) == 0;
}

int first_part_2021_8() {
    std::vector<size_t> all_numbers;
    bool is_output_section = false;
    std::transform(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), std::inserter(all_numbers, all_numbers.end()), [&] (auto str) {
        size_t res = 0;
        std::string final_number;
        std::string line;
        std::stringstream s(str);
        std::getline(s, line, '|');

        std::vector<std::string> all_numbers;
        std::map<size_t, std::string> known_numbers;
        // found numbers: 0, 1, 4, 5, 6, 7, 8, 9

        std::stringstream first_part("");
        first_part.str(line);
        while (std::getline(first_part, line, ' ')) {
            if (line.empty()) continue;
            std::sort(line.begin(), line.end());
            all_numbers.push_back(line);
            if (line.size() == 2) {
                known_numbers[1] = line;
            }
            if (line.size() == 4) {
                known_numbers[4] = line;
            }
            if (line.size() == 3) {
                known_numbers[7] = line;
            }
            if (line.size() == 7) {
                known_numbers[8] = line;
            }
        }
        auto top_segment = find_diff(known_numbers[1], known_numbers[7]); // 7 - 1
        char top_right_segment;
        char bottom_left_segment;
        char center_segment;
        auto possible_6 = find_number(all_numbers, 6, known_numbers); // (possible: 0, 6, 9)
        auto it = possible_6.begin();
        while (it != possible_6.end()) { // 7 - 6
            if (!is_all_segment_exists(*it, known_numbers[7])) {
                top_right_segment = find_diff(*it, known_numbers[7]);
                known_numbers[6] = *it;
                break;
            }
            it++;
        }
        auto possible_3 = find_number(all_numbers, 3, known_numbers); // 2, 3, 5
        it = possible_3.begin();
        while (it != possible_3.end()) { // 5 - 2
            auto ans = is_all_segment_exists(*it, known_numbers[1]);
            if (ans) {
                known_numbers[3] = *it;
                break;
            }
            it++;
        }
        auto possible_5 = find_number(all_numbers, 5, known_numbers); // 2, 5
        it = possible_5.begin();
        while (it != possible_5.end()) { // 6 - 5
            auto segment = find_diff(*it, known_numbers[6]);
            if (is_all_segment_exists(*it, known_numbers[6])) {
                known_numbers[5] = *it;
                bottom_left_segment = segment;
            } else {
                known_numbers[2] = *it;
            }
            it++;
        }
        auto possible_9 = find_number(all_numbers, 9, known_numbers); // 0, 9
        it = possible_9.begin();
        while (it != possible_9.end()) { // 8 - 9
            auto segment = find_diff(*it, known_numbers[8]);
            if (segment == bottom_left_segment) {
                known_numbers[9] = *it;
            } else {
                center_segment = segment;
                known_numbers[0] = *it;
            }
            it++;
        }
        /*auto possible_2 = find_number(all_numbers, 2, known_numbers); // 2, 3
        it = possible_2.begin();
        while (it != possible_2.end()) { // 1 - 2
            auto ans = is_all_segment_exists(*it, known_numbers[1]);
            if (ans) {
                known_numbers[3] = *it;
            } else {
                known_numbers[2] = *it;
            }
            it++;
        }*/

        std::getline(s, line, '|');

        std::stringstream second_part("");
        second_part.str(line);
        while (std::getline(second_part, line, ' ')) {
            if (line.empty()) continue;
            std::sort(line.begin(), line.end());
            final_number += std::to_string(parse_combination(line, known_numbers));
        }

        return std::stoll(final_number);
    });

    std::cout << "size: " << std::reduce(all_numbers.begin(), all_numbers.end()) << std::endl;

    return EXIT_SUCCESS;
}

#endif