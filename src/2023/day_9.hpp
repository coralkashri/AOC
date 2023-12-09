#ifndef AOC_2023_9
#define AOC_2023_9

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include <array>
#include <set>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

int64_t predict_number(bool predict_future, std::string_view str, auto &&prediction_func) {
    size_t prediction = 0;
    str_numbers<int64_t> numbers(str);
    std::vector<std::vector<int64_t>> modification_lines(1, numbers.get());
    std::vector<int64_t> numbers_for_predict(1, predict_future ? modification_lines.back().back()
                                                               : modification_lines.back().front());

    while (!std::ranges::all_of(modification_lines.back(), [](int64_t n) { return n == 0; })) {
        auto &current_modifications = modification_lines.emplace_back();
        auto &current_numbers = *(modification_lines.rbegin() + 1);
        current_modifications.reserve(current_numbers.size() - 1);
        for (size_t i = 0; i < current_numbers.size() - 1; ++i) {
            current_modifications.push_back(current_numbers[i + 1] - current_numbers[i]);
        }
        numbers_for_predict.push_back(predict_future ? current_modifications.back() : current_modifications.front());
    }

    return prediction_func(numbers_for_predict);
}

int64_t solve(bool predict_future, auto &&prediction_func) {
    return std::accumulate(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE),
                           std::istream_iterator<WordDelimitedBy<'\n'>>(),
                           0ll, [predict_future, &prediction_func](int64_t res, std::string str) {
                return res + predict_number(predict_future, str, prediction_func);
            });
}

int first_part_2023_9() {
    std::cout << solve(true, [](const auto &prediction_numbers) {
        return std::reduce(prediction_numbers.begin(), prediction_numbers.end(), 0ll);
    }) << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2023_9() {
    std::cout << solve(false, [](const auto &prediction_numbers) {
        int64_t prev_number = 0;
        for (int64_t i = ssize(prediction_numbers) - 1; i >= 0; --i) {
            prev_number = prediction_numbers[i] - prev_number;
        }
        return prev_number;
    }) << std::endl;

    return EXIT_SUCCESS;
}

#endif