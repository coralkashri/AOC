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

int count_dots(std::vector<std::vector<size_t>> &dots) {
    size_t res = 0;
    for (auto line : dots) {
        res += std::count(line.begin(), line.end(), 1);
    }
    return res;
}

void fix_missing_lines(std::vector<std::vector<size_t>> &dots, int lines) {
    if (dots.size() < lines) dots.resize(lines);
}

void fix_missing_rows(std::vector<std::vector<size_t>> &dots, int rows) {
    for (auto &line : dots) {
        if (line.size() < rows) line.resize(rows, 0);
    }
}

int first_part_2021_13() {
    std::vector<std::vector<size_t>> dots;
    //std::map<std::pair<size_t, size_t>, bool> new_dots;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (auto line) {
        if (line.empty()) return;
        if (auto it = std::find(line.begin(), line.end(), '='); it != line.end()) {
            if (*(it - 1) == 'y') {
                int fold_line = std::stoi(line.substr(std::distance(line.begin(), it + 1)));
                size_t current_line = fold_line;
                std::for_each(dots.begin() + fold_line, dots.end(), [&] (auto &line) {
                    for (size_t i = 0; i < line.size(); i++) {
                        if (line[i]) dots[(fold_line - current_line % fold_line) % fold_line][i] = line[i];
                        line[i] = 0;
                    }
                    current_line++;
                });
                std::cout << count_dots(dots) << std::endl;
            } else {
                int fold_line = std::stoi(line.substr(std::distance(line.begin(), it + 1)));
                std::for_each(dots.begin(), dots.end(), [&] (auto &line) {
                    for (size_t i = fold_line; i < line.size(); i++) {
                        if (line[i]) line[(fold_line - i % fold_line) % fold_line] = line[i];
                        line[i] = 0;
                    }
                });
                std::cout << count_dots(dots) << std::endl;
            }
        } else {
            auto sep = std::distance(line.begin(), std::find(line.begin(), line.end(), ','));
            int x = std::stoi(line.substr(0, sep));
            int y = std::stoi(line.substr(sep + 1));
            fix_missing_lines(dots, y + 1);
            fix_missing_rows(dots, x + 1);
            dots.at(y).at(x) = 1;
        }
    });

    // Part 2 result
    for (auto line : dots) {
        for (auto dot : line) {
            std::cout << (dot == 1 ? '#' : '.');
        }
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}

#endif