#ifndef AOC_2022_3
#define AOC_2022_3

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

int first_part_2022_3() {
    std::cout << std::accumulate(std::istream_iterator<std::string>(INPUT_SOURCE),
                                 std::istream_iterator<std::string>(),
                                 0ull, [](size_t res, const std::string &rucksack) {
                int priority = 0;
                std::set<char> first_half(rucksack.begin(), rucksack.begin() + rucksack.size() / 2);
                std::set<char> second_half(rucksack.begin() + rucksack.size() / 2, rucksack.end());

                std::ranges::set_intersection(first_half, second_half, &priority);

                priority = std::islower(priority) ? priority - 'a' + 1 : priority - 'A' + 27;

                return res + priority;
            }) << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2022_3() {
    std::array<std::vector<char>, 3> group;
    std::vector<int> priority = {0};
    priority.reserve(100);
    std::cout << std::accumulate(std::istream_iterator<std::string>(INPUT_SOURCE),
                                 std::istream_iterator<std::string>(),
                                 0ull, [group_index = 0, &group, &priority](size_t res, const std::string &rucksack) mutable {
                priority[0] = 0;
                group[group_index].insert(group[group_index].begin(), rucksack.begin(), rucksack.end());

                if (++group_index == 3) {
                    group_index = 0;
                    priority.clear();

                    std::sort(group[0].begin(), group[0].end());
                    std::sort(group[1].begin(), group[1].end());
                    std::sort(group[2].begin(), group[2].end());

                    static std::vector<char> common_between_two;
                    std::ranges::set_intersection(group[0], group[1], std::back_inserter(common_between_two));
                    std::ranges::set_intersection(common_between_two, group[2], std::back_inserter(priority));
                    priority[0] = std::islower(priority[0]) ? priority[0] - 'a' + 1 : priority[0] - 'A' + 27;

                    group[0].clear();
                    group[1].clear();
                    group[2].clear();
                    common_between_two.clear();
                }

                return res + priority[0];
            }) << std::endl;

    return EXIT_SUCCESS;
}

#endif