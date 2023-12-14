#ifndef AOC_2023_14
#define AOC_2023_14

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include <array>
#include <set>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

int first_part_2023_14() {
	std::cout << std::transform_reduce(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), 0ull, std::plus<>(), [weights = std::vector<size_t>(), current_line_weight = 0ull](std::string str) mutable {
		if (weights.empty()) {
			weights.resize(str.size(), str.size());
			current_line_weight = str.size();
		}
		size_t res = 0;
		size_t col = 0;
		for (char c : str) {
			if (c == '#') weights[col] = current_line_weight - 1;
			else if (c == 'O') res += weights[col]--;
			++col;
		}
		--current_line_weight;
		return res;
	}) << std::endl;
    return EXIT_SUCCESS;
}

enum move_directions {
	UP,
	LEFT,
	DOWN,
	RIGHT
};

template <move_directions Direction>
void move(std::vector<std::string>& matrix) {
	auto target_new = std::vector<size_t>(matrix.size(), Direction == UP || Direction == LEFT ? 0 : matrix.size() - 1);

	int direction_movement = Direction == UP || Direction == LEFT ? 1 : -1;

	for (int64_t y = Direction != DOWN ? 0 : (int64_t)matrix.size() - 1; Direction != DOWN ? y < matrix.size() : y >= 0; Direction != DOWN ? ++y : --y) {
		for (int64_t x = Direction != RIGHT ? 0 : (int64_t)matrix[0].size() - 1; Direction != RIGHT ? x < matrix[0].size() : x >= 0; Direction != RIGHT ? ++x : --x) {
			if (matrix[y][x] == '#') target_new[Direction % 2 == 0 ? x : y] = (Direction % 2 == 0 ? y : x) + direction_movement;
			if (matrix[y][x] == 'O') std::swap(matrix[y][x], Direction % 2 == 0 ? matrix[(target_new[x] += direction_movement) - direction_movement][x] : matrix[y][(target_new[y] += direction_movement) - direction_movement]);
		}
	}
}

void apply_cycle(std::vector<std::string>& matrix) {
	move<UP>(matrix);
	move<LEFT>(matrix);
	move<DOWN>(matrix);
	move<RIGHT>(matrix);
}

size_t calculate_north_weight(const std::vector<std::string>& matrix) {
	size_t res = 0;
	for (size_t y = 0; y < matrix.size(); ++y) {
		for (size_t x = 0; x < matrix[0].size(); ++x) {
			if (matrix[y][x] == 'O') res += matrix.size() - y;
		}
	}
	return res;
}

int second_part_2023_14() {
	std::vector<std::string> matrix;
	std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [&matrix](std::string str) mutable {
		matrix.push_back(str);
		});

	std::vector<int64_t> results;
	std::vector<int64_t> results_loop;
	size_t i = 0;
	size_t results_vector_start_index = 0;
	for (; i < 1000; ++i) {
		apply_cycle(matrix);
		int64_t res = calculate_north_weight(matrix);
		results.push_back(res);

		if (results.size() > 10) {
			if (auto it = std::search(results.begin(), std::prev(results.end(), 10), std::prev(results.end(), 10), results.end()); it != std::prev(results.end(), 10)) {
				std::cout << "Loop found!: " << *it << " " << *(it + 1) << " " << *(it + 2) << std::endl;
				results_loop = std::vector<int64_t>(it, std::prev(results.end(), 10));
				break;
			}
		}
	}

	std::cout << results_loop[(9 + 1000000000 - 1 - i) % results_loop.size()] << std::endl;

    return EXIT_SUCCESS;
}

#endif