#ifndef AOC_2023_13
#define AOC_2023_13

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include <array>
#include <set>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

size_t count_bits_error(size_t n1, size_t n2) {
    size_t error = n1 ^ n2;
    size_t count = 0;
    while (error) {
        count += error & 1;
        error >>= 1;
    }
    return count;
}

template <size_t BitErrorsCount>
uint64_t find_mirror(const auto &rows, const auto &columns) {
    size_t max_mirror_index = 0;
    size_t max_mirror_effect = 0;
    bool is_mirror_rows = true;

    auto find_biggest_mirror = [&max_mirror_index, &max_mirror_effect, &is_mirror_rows](const auto& range, bool is_rows) {
        for (int64_t i = 0; i < ssize(range) - 1; ++i) {
            size_t current_mirrored_rows = 0;
            size_t bits_error = 0;
            if (range[i] == range[i + 1] || BitErrorsCount && (bits_error = count_bits_error(range[i], range[i + 1])) == BitErrorsCount) {
                ++current_mirrored_rows;
                for (int64_t j = i + 2; j < ssize(range); ++j) {
                    int64_t mirror_index = j - (j - i) * 2 + 1;
                    if (mirror_index < 0) break;
                    if (range[j] != range[mirror_index] && (!BitErrorsCount || (bits_error += count_bits_error(range[j], range[mirror_index])) > BitErrorsCount)) break;
                    current_mirrored_rows++;
                }
            }
            if (bits_error == BitErrorsCount && current_mirrored_rows > max_mirror_effect &&
                (i + current_mirrored_rows == range.size() - 1 || i - current_mirrored_rows + 1 == 0)) {
                max_mirror_effect = current_mirrored_rows;
                max_mirror_index = i;
                is_mirror_rows = is_rows;
            }
        }
    };

    find_biggest_mirror(rows, true);
    find_biggest_mirror(columns, false);

    return (max_mirror_index + 1) * (is_mirror_rows ? 100 : 1);
}

void read_new_line(auto& rows, auto& columns, std::string_view str) {
    if (columns.empty()) {
        columns.resize(str.size(), 0);
    }

    rows.emplace_back();

    auto columns_it = columns.begin();
    for (char c: str) {
        *columns_it <<= 1;
        *columns_it++ |= c == '#';

        rows.back() <<= 1;
        rows.back() |= c == '#';
    }
}

template <size_t BitErrorsCount>
size_t solve() {
    std::vector<size_t> rows; // >
    std::vector<size_t> columns; // V
    return std::transform_reduce(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE),
                                       std::istream_iterator<WordDelimitedBy<'\n'>>(),
                                       0ull, std::plus<>(), [&rows, &columns](std::string str) {
                if (str.empty()) {
                    size_t res = find_mirror<BitErrorsCount>(rows, columns);
                    rows.clear();
                    columns.clear();
                    return res;
                }

                read_new_line(rows, columns, str);

                return 0ul;
            }) + find_mirror<BitErrorsCount>(rows, columns);
}

int first_part_2023_13() {
    std::cout << solve<0>() << std::endl;
    return EXIT_SUCCESS;
}

int second_part_2023_13() {
    std::cout << solve<1>() << std::endl;
    return EXIT_SUCCESS;
}

#endif