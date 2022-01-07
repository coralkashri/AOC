#ifndef AOC_2020_10
#define AOC_2020_10

#include "../tools/base_includes.h"

void input_numbers(std::set<size_t> &numbers) {
    std::transform(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), std::inserter(numbers, numbers.end()), [&] (std::string str) mutable {
        return std::stol(str);
    });
}

auto count_diff_1_3(const auto &numbers) {
    size_t diff_1_counter, diff_3_counter;
    diff_1_counter = diff_3_counter = 1;
    std::for_each(std::next(numbers.begin(), 1), numbers.end(), [prev_num = *numbers.begin(), &diff_1_counter, &diff_3_counter] (const auto &current_number) mutable {
        if (current_number - std::exchange(prev_num, current_number) == 1) diff_1_counter++;
        else diff_3_counter++;
    });
    return std::pair{diff_1_counter, diff_3_counter};
}

int first_part_2020_10() {
    std::set<size_t> numbers;
    input_numbers(numbers);

    auto [diff_1_counter, diff_3_counter] = count_diff_1_3(numbers);

    std::cout << diff_1_counter * diff_3_counter << std::endl;

    return EXIT_SUCCESS;
}

void find_diff_1_groups(const auto &numbers, auto &diff_1_groups) {
    std::for_each(numbers.begin(), numbers.end(), [prev_num = 0, &diff_1_groups] (const auto &current_number) mutable {
        if (current_number - std::exchange(prev_num, current_number) == 1) diff_1_groups.back()++;
        else {
            if (diff_1_groups.back() > 1) {
                diff_1_groups.template emplace_back();
            } else diff_1_groups.back() = 0;
        }
    });

    if (diff_1_groups.back() <= 1) {
        diff_1_groups.pop_back();
    }
}

void update_groups_possibilities(auto &diff_1_groups) {
    std::transform(diff_1_groups.begin(), diff_1_groups.end(), diff_1_groups.begin(), [] (const auto &diff_group_count) {
        return std::pow(2, diff_group_count - 1) - (diff_group_count == 4);
    });
}

uint64_t compute_possible_ways(const auto &possibilities_groups) {
    return std::reduce(possibilities_groups.begin(), possibilities_groups.end(), 1ull, std::multiplies<>{});
}

int second_part_2020_10() {
    std::set<size_t> numbers;
    std::list<size_t> diff_1_groups_counter(1);
    input_numbers(numbers);

    find_diff_1_groups(numbers, diff_1_groups_counter);

    update_groups_possibilities(diff_1_groups_counter);

    std::cout << compute_possible_ways(diff_1_groups_counter) << std::endl;

    return EXIT_SUCCESS;
}

#endif