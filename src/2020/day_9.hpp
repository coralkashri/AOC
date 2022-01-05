#ifndef AOC_2020_9
#define AOC_2020_9

#include <iostream>
#include <chrono>
#include <unordered_map>
#include <set>
#include <algorithm>
#include "../input_source.hpp"

bool validate_num(const auto &numbers, auto num) {
    return std::find_if(numbers.begin(), numbers.end(), [&] (auto pair) {
        return numbers.find(num - pair.first) != numbers.end();
    }) != numbers.end();
}

void update_min_max_values_by_range(auto start_it, auto stop_it, size_t &min, size_t &max) {
    const auto [min_it, max_it] = std::minmax_element(start_it, stop_it);
    min = *min_it;
    max = *max_it;
}

template <typename T>
void find_sub_list_of_sum_x(const auto &numbers, T desired_sum, T &highest_sum_number, T &lowest_sum_number) {
    auto beg = numbers.begin();
    auto end = std::next(beg);
    T sum = *beg;
    highest_sum_number = lowest_sum_number = *beg;
    while (end != numbers.end()) {
        sum += *end;
        highest_sum_number = std::max(highest_sum_number, *end);
        lowest_sum_number = std::min(lowest_sum_number, *end);
        if (sum == desired_sum) break; // Sum found
        if (sum > desired_sum) {
            sum -= *beg++;
            while (end != std::next(beg) && sum > desired_sum) sum -= *end--;
            update_min_max_values_by_range(beg, std::next(end), lowest_sum_number, highest_sum_number);
            if (sum == desired_sum) break; // Sum found
            /*// Alternative (potentially less efficiently):
            sum = *++beg;
            highest_sum_number = lowest_sum_number = *beg;
            end = beg;*/
        }
        end++;
    }
    if (sum != desired_sum) {
        highest_sum_number = lowest_sum_number = 0;
    }
}

auto find_first_illegal_number(std::vector<size_t> &all_valid_numbers) {
#if TEST_RUN
    const size_t preamble_size = 5;
#else
    const size_t preamble_size = 25;
#endif
    std::vector<std::pair<size_t, bool>> numbers;
    std::unordered_map<size_t, bool> numbers_map;

    std::generate_n(std::inserter(numbers, numbers.end()), preamble_size, [] () { return std::pair{std::stoll(*std::istream_iterator<std::string>(INPUT_SOURCE)), true}; });
    std::transform(numbers.begin(), numbers.end(), std::back_inserter(all_valid_numbers), [] (const auto &pair) { return pair.first; });
    numbers_map.insert(numbers.begin(), numbers.end());

    return std::find_if(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [&] (auto str) {
        auto current_number = std::stoll(str);
        auto ans = !validate_num(numbers_map, current_number);
        if (!ans) {
            all_valid_numbers.template emplace_back(current_number);
            std::rotate(numbers.begin(), std::next(numbers.begin()), numbers.end());
            numbers_map.erase(std::prev(numbers.end())->first);
            std::prev(numbers.end())->first = current_number;
            numbers_map.template insert(std::pair{current_number, true});
        }
        return ans;
    });
}

int first_part_2020_9() {
    std::vector<size_t> all_valid_numbers;
    std::cout << "First illegal number: " << *find_first_illegal_number(all_valid_numbers) << std::endl;
    return EXIT_SUCCESS;
}

int second_part_2020_9() {
    std::vector<size_t> all_valid_numbers;
    auto first_illegal_number_iter = find_first_illegal_number(all_valid_numbers);

    size_t highest_num_of_sum, lowest_num_of_sum;
    size_t first_illegal_number = std::stoll(*first_illegal_number_iter);
    find_sub_list_of_sum_x(all_valid_numbers, first_illegal_number, highest_num_of_sum, lowest_num_of_sum);
    std::cout << "highest_num_of_sum + lowest_num_of_sum: " << (highest_num_of_sum + lowest_num_of_sum) << std::endl;
    return EXIT_SUCCESS;
}

#endif