#ifndef AOC_2021_13
#define AOC_2021_13

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

auto count_dots(std::vector<std::vector<size_t>> &matrix) {
    size_t res = 0;
    for (auto line : matrix) {
        res += std::count(line.begin(), line.end(), 1);
    }
    return res;
}

void fix_missing_lines(std::vector<std::vector<size_t>> &matrix, int lines) {
    if (matrix.size() < lines) matrix.resize(lines);
}

void fix_missing_rows(std::vector<std::vector<size_t>> &matrix, int rows) {
    for (auto &line : matrix) {
        if (line.size() < rows) line.resize(rows, 0);
    }
}

void fold_by_line(int fold_line, auto &matrix) {
    size_t current_line = fold_line;
    std::for_each(matrix.begin() + fold_line, matrix.end(), [&](auto &line) {
        for (size_t i = 0; i < line.size(); i++) {
            if (line[i]) matrix[(fold_line - current_line % fold_line) % fold_line][i] = line[i];
            line[i] = 0;
        }
        current_line++;
    });
    matrix.resize(fold_line);
    std::cout << count_dots(matrix) << std::endl;
}

void fold_by_row(int fold_row, auto &matrix) {
    std::for_each(matrix.begin(), matrix.end(), [&](auto &line) {
        for (size_t i = fold_row; i < line.size(); i++) {
            if (line[i]) line[(fold_row - i % fold_row) % fold_row] = line[i];
            line[i] = 0;
        }
        line.resize(fold_row);
    });
    std::cout << count_dots(matrix) << std::endl;
}

void add_dot_to_matrix(auto& matrix, size_t x, size_t y) {
    fix_missing_lines(matrix, y + 1);
    fix_missing_rows(matrix, x + 1);
    matrix.at(y).at(x) = 1;
}

int first_part_2021_13() {
    std::vector<std::vector<size_t>> matrix;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (auto line) {
        if (line.empty()) return;
        if (auto it = std::find(line.begin(), line.end(), '='); it != line.end()) {
            if (*(it - 1) == 'y') {
                    int fold_line = std::stoi(line.substr(std::distance(line.begin(), it + 1)));
                fold_by_line(fold_line, matrix);
            } else {
                int fold_row = std::stoi(line.substr(std::distance(line.begin(), it + 1)));
                fold_by_row(fold_row, matrix);
            }
        } else {
            auto sep = std::distance(line.begin(), std::find(line.begin(), line.end(), ','));
            int x = std::stoi(line.substr(0, sep));
            int y = std::stoi(line.substr(sep + 1));
            add_dot_to_matrix(matrix, x, y);
        }
    });

    // Part 2 result
    for (auto &line : matrix) {
        for (auto dot : line) {
            std::cout << (dot == 1 ? "###" : "   ");
        }
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}

#endif