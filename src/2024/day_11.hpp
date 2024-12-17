#ifndef ADVENTOFCODE2024_DAY_11_HPP
#define ADVENTOFCODE2024_DAY_11_HPP

#include <iostream>
#include <algorithm>
#include <iterator>
#include <map>
#include <ranges>
#include <numeric>
#include <regex>
#include <cmath>

#include "../input_source.hpp"
#include "../tools/str_number.hpp"
#include "../tools/word_delimited_by.h"
#include "../tools/matrix.hpp"
#include "../tools/point.hpp"
#include <set>
#include "../tools/range.hpp"

class researcher {
private:
    struct research_data {
        bool research_complete = false;
        int64_t iterations_to_split = -1;
        std::pair<int64_t, int64_t> split_numbers;
        int64_t number = -1;

        bool operator<(const research_data& ref) const {
            if (iterations_to_split < ref.iterations_to_split) return true;
            if (iterations_to_split > ref.iterations_to_split) return false;
            if (split_numbers.first < ref.split_numbers.first) return true;
            if (split_numbers.first > ref.split_numbers.first) return false;
            if (split_numbers.second < ref.split_numbers.second) return true;
            if (split_numbers.second > ref.split_numbers.second) return false;
            if (number < ref.number) return true;
            return false;
        }
    };

public:
    size_t compute(std::span<int> numbers, int64_t required_iterations) {
        std::map<research_data, size_t> data;

        for (auto n : numbers) {
            auto &r = conclusions[n];
            if (!r.research_complete) {
                r = start_research(n);
            }
            ++data[r];
        }

        while (required_iterations > 0) {
            int64_t forward_iterations = data.begin()->first.iterations_to_split + 1;
            std::map<research_data, size_t> new_data;
            for (auto& [r, count] : data) {
                if (r.iterations_to_split - forward_iterations < 0) {
                    auto& new_r1 = conclusions[r.split_numbers.first];
                    auto& new_r2 = conclusions[r.split_numbers.second];
                    if (!new_r1.research_complete) new_r1 = start_research(r.split_numbers.first);
                    if (!new_r2.research_complete) new_r2 = start_research(r.split_numbers.second);
                    new_data[new_r1] += count;
                    new_data[new_r2] += count;
                } else {
                    new_data[{
                        .research_complete = true,
                        .iterations_to_split = static_cast<uint8_t>(r.iterations_to_split - forward_iterations),
                        .split_numbers = r.split_numbers,
                        .number = (r.number == 0) ? 1 : r.number * 2024
                    }] += count;
                }
            }

            required_iterations -= forward_iterations;
            std::swap(data, new_data);
        }

        return std::transform_reduce(data.begin(), data.end(), 0ull, std::plus<>{}, [](auto &val) {
            //std::cout << val.first.number << "(" << val.second << ") ";
            return val.second;
        });
    }

private:
    std::unordered_map<int64_t, research_data> conclusions;

    [[nodiscard]] int64_t digits_count(int64_t number) const {
        int64_t counter = 0;
        while (number != 0) {
            number /= 10;
            ++counter;
        }

        return counter;
    }

    research_data start_research(int64_t number) {
        uint8_t iterations = 0;
        uint8_t digits;
        int64_t source_number = number;

        if (!number) {
            iterations = 1;
            number = 1;
        }

        while ((digits = digits_count(number)) % 2) {
            number *= 2024;
            ++iterations;
        }

        return research_data{
                .research_complete = true,
                .iterations_to_split = iterations,
                .split_numbers = split_number(number, digits),
                .number = source_number
        };
    }

    [[nodiscard]] std::pair<int64_t, int64_t> split_number(int64_t number, const uint8_t digits_count) const {
        const uint8_t half_digits_count = digits_count / 2;
        std::pair<int64_t, int64_t> split_numbers = {0, 0};
        int64_t tens_second = static_cast<int64_t>(std::pow(10, digits_count - 1));

        split_numbers.second = number % static_cast<int64_t>(std::pow(10, half_digits_count));

        for (uint8_t digit_number = 0; digit_number < half_digits_count; ++digit_number) {
            split_numbers.first *= 10;
            split_numbers.first += number / tens_second;
            number -= (split_numbers.first % 10) * tens_second;
            tens_second /= 10;
        }

        return split_numbers;
    }
};

int first_part_2024_11() {
    std::string numbers_str;
    std::getline(INPUT_SOURCE, numbers_str, '\n');
    auto numbers = str_numbers<int, std::vector, ' '>(numbers_str).get();
    researcher r;
    std::cout << r.compute(numbers, 25) << "\n";

    return EXIT_SUCCESS;
}

int second_part_2024_11() {
    std::string numbers_str;
    std::getline(INPUT_SOURCE, numbers_str, '\n');
    auto numbers = str_numbers<int, std::vector, ' '>(numbers_str).get();
    researcher r;
    std::cout << r.compute(numbers, 75) << "\n";

    return EXIT_SUCCESS;
}

#endif
