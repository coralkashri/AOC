#ifndef AOC_2023_3
#define AOC_2023_3

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

size_t get_complete_number(const std::vector<std::string>& matrix, size_t i, size_t &j) {
    while (std::isdigit(matrix[i][j])) {
        --j;
    }

    str_number number;
    while (std::isdigit(matrix[i][++j])) {
        number += matrix[i][j];
    }

    return number.get_number();
}

size_t sum_neighbors(const std::vector<std::string>& matrix, size_t i, size_t j) {
    size_t sum = 0;
    for (size_t ni = i - 1; ni <= i + 1; ++ni) {
        for (size_t nj = j - 1; nj <= j + 1; ++nj) {
            if (std::isdigit(matrix[ni][nj])) {
                sum += get_complete_number(matrix, ni, nj);
            }
        }
    }
    return sum;
}

size_t mul_two_neighbors(const std::vector<std::string>& matrix, size_t i, size_t j) {
    std::array<size_t, 2> neighbors{0, 0};
    size_t current_neighbor = 0;
    for (size_t ni = i - 1; ni <= i + 1; ++ni) {
        for (size_t nj = j - 1; nj <= j + 1; ++nj) {
            if (std::isdigit(matrix[ni][nj])) {
                if (current_neighbor == 2) return 0;
                neighbors[current_neighbor++] = get_complete_number(matrix, ni, nj);
            }
        }
    }
    return current_neighbor == 2 ? neighbors[0] * neighbors[1] : 0;
}

int first_part_2023_3() {
    std::vector<std::string> matrix;
    std::string line;
    while (std::getline(INPUT_SOURCE, line)) {
        matrix.push_back('.' + line + '.');
    }
    std::fill(line.begin(), line.end(), '.');
    matrix.insert(matrix.begin(), line);
    matrix.push_back(line);

    size_t sum = 0;
    for (size_t i = 1; i < matrix.size() - 1; ++i) {
        for (size_t j = 1; j < line.size() - 1; ++j) {
            if (matrix[i][j] != '.' && !std::isdigit(matrix[i][j])) {
                sum += sum_neighbors(matrix, i, j);
            }
        }
    }

    std::cout << sum << std::endl;
    return EXIT_SUCCESS;
}

int second_part_2023_3() {
    std::vector<std::string> matrix;
    std::string line;
    while (std::getline(INPUT_SOURCE, line)) {
        matrix.push_back('.' + line + '.');
    }
    std::fill(line.begin(), line.end(), '.');
    matrix.insert(matrix.begin(), line);
    matrix.push_back(line);

    size_t sum = 0;
    for (size_t i = 1; i < matrix.size() - 1; ++i) {
        for (size_t j = 1; j < line.size() - 1; ++j) {
            if (matrix[i][j] == '*') {
                sum += mul_two_neighbors(matrix, i, j);
            }
        }
    }

    std::cout << sum << std::endl;
    return EXIT_SUCCESS;
}

#endif