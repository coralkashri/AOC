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
#include "../tools/word_delimited_by.h"

bool is_combination_exists(std::string comb, std::map<size_t, std::string> comb_map) {
	return std::find_if(comb_map.begin(), comb_map.end(), [comb] (auto pair) {
		return pair.second == comb;
	}) != comb_map.end();
}

std::vector<std::string> find_number(auto numbers, size_t number, auto known_numbers) {
	std::vector<std::string> res;
	if (auto it = known_numbers.find(number); it != known_numbers.end()) {
		res.push_back(it->second);
		return res;
	}
	std::for_each(numbers.begin(), numbers.end(), [&] (auto str) {
		if (is_combination_exists(str, known_numbers)) return;
		if (str.size() == 6 && (number == 0 || number == 6 || number == 9)) res.push_back(str);
		if (str.size() == 5 && (number == 2 || number == 3 || number == 5)) res.push_back(str);
	});
	return res;
}

size_t parse_combination(std::string comb, auto comb_map) {
	return std::find_if(comb_map.begin(), comb_map.end(), [comb] (auto pair) {
		return pair.second == comb;
	})->first;
}

char find_diff(std::string a, std::string b) {
	for (auto segment_b : b) {
		bool is_exists = false;
		for (auto segment_a : a) {
			if (segment_a == segment_b) {
				is_exists = true;
				break;
			}
		}
		if (!is_exists) return segment_b;
	}
	return '-';
}

char diff_count(std::string a, std::string b) {
	size_t res = 0;
	for (auto segment_b : b) {
		bool is_exists = false;
		for (auto segment_a : a) {
			if (segment_a == segment_b) {
				is_exists = true;
				break;
			}
		}
		if (!is_exists) res++;
	}
	return res;
}

bool is_all_segment_exists(std::string a, std::string b) {
	return find_diff(a, b) == '-';
}

int first_part_2021_8() {
    std::vector<size_t> all_numbers;
	std::transform(std::istream_iterator<WordDelimitedBy<'\n'>>(std::cin), std::istream_iterator<WordDelimitedBy<'\n'>>(), std::back_inserter(all_numbers), [&] (auto str) {
		size_t res;
		std::string final_number;
		std::string line;
		std::stringstream s(str);
		std::getline(s, line, '|');
		
		std::vector<std::string> all_numbers;
		std::map<size_t, std::string> known_numbers;
		
		std::stringstream first_part("");
		first_part.str(line);
		while (std::getline(first_part, line, ' ')) {
			if (line.empty()) continue;
			std::sort(line.begin(), line.end());
			if (line.size() == 2) {
				known_numbers[1] = line;
			} else
			if (line.size() == 4) {
				known_numbers[4] = line;
			} else
			if (line.size() == 3) {
				known_numbers[7] = line;
			} else
			if (line.size() == 7) {
				known_numbers[8] = line;
			} else all_numbers.push_back(line);
		}
		auto top_segment = find_diff(known_numbers[1], known_numbers[7]);
		char top_right_segment;
		char bottom_left_segment;
		char center_segment;
		auto possible_6 = find_number(all_numbers, 6, known_numbers); // 0, 6, 9
		auto it = possible_6.begin();
		while (it != possible_6.end()) {
			if (!is_all_segment_exists(*it, known_numbers[1])) {
				top_right_segment = find_diff(*it, known_numbers[1]);
				known_numbers[6] = *it;
				all_numbers.erase(std::find(all_numbers.begin(), all_numbers.end(), *it));
				break;
			}
			it++;
		}
		
		auto possible_3 = find_number(all_numbers, 3, known_numbers); // 2, 3, 5
		it = possible_3.begin();
		while (it != possible_3.end()) {
			if (is_all_segment_exists(*it, known_numbers[1])) {
				known_numbers[3] = *it;
				all_numbers.erase(std::find(all_numbers.begin(), all_numbers.end(), *it));
				break;
			}
			it++;
		}
		
		auto possible_5 = find_number(all_numbers, 5, known_numbers); // 2, 5
		it = possible_5.begin();
		while (it != possible_5.end()) {
			auto segment = diff_count(*it, known_numbers[4]);
			if (segment == 1) {
				known_numbers[5] = *it;
				all_numbers.erase(std::find(all_numbers.begin(), all_numbers.end(), *it));
				bottom_left_segment = find_diff(*it, known_numbers[4]);
			} else {
				known_numbers[2] = *it;
				all_numbers.erase(std::find(all_numbers.begin(), all_numbers.end(), *it));
			}
			it++;
		}
		
		auto possible_9 = find_number(all_numbers, 9, known_numbers);
		it = possible_9.begin();
		while (it != possible_9.end()) {
			auto segment = diff_count(*it, known_numbers[3]);
			if (segment == 0) {
				known_numbers[9] = *it;
				all_numbers.erase(std::find(all_numbers.begin(), all_numbers.end(), *it));
			} else {
				known_numbers[0] = *it;
				all_numbers.erase(std::find(all_numbers.begin(), all_numbers.end(), *it));
			}
			it++;
		}
		
		std::getline(s, line, '|');
		std::stringstream second_part("");
		second_part.str(line);
		
		while (std::getline(second_part, line, ' ')) {
			if (line.empty()) continue;
			std::sort(line.begin(), line.end());
			final_number += std::to_string(parse_combination(line, known_numbers));
		}
		if (final_number.empty()) return 0ll;
		return std::stoll(final_number);
	});
	
	std::cout << "Number: " << std::reduce(all_numbers.begin(), all_numbers.end()) << std::endl;

    return EXIT_SUCCESS;
}

#endif
