#ifndef AOC_2020_1
#define AOC_2020_1

#include <iostream>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>
#include "../input_source.hpp"

int first_part_2020_1() {
    const size_t target_num = 2020;
    std::unordered_map<int, bool> map;
    std::transform(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), std::inserter(map, map.end()), [] (auto str) {
        return std::pair{std::stoi(str), true};
    });
    auto it = std::find_if(map.begin(), map.end(), [target_num, &map] (const auto &map_pair) {
        return map.find(target_num - map_pair.first) != map.end();
    });
    std::cout << "Multiplication result: " << (it->first * (target_num - it->first)) << std::endl;
    return EXIT_SUCCESS;
}

int second_part_2020_1() {
    const size_t target_num = 2020;
    std::map<int, bool> numbers;
    std::map<int, bool>::iterator p1;
    std::transform(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), std::inserter(numbers, numbers.end()), [] (auto str) {
        return std::pair{std::stoi(str), true};
    });
    p1 = numbers.begin();
    while (p1 != std::next(numbers.begin(), numbers.size() / 2)) {
        auto it = std::find_if(std::next(p1), numbers.end(), [target_num = target_num - p1->first, &numbers] (const auto &map_pair) {
            return numbers.find(target_num - map_pair.first) != numbers.end();
        });
        if (it != numbers.end()) {
            std::cout << "Multiplication result: " << (p1->first * it->first * (target_num - it->first - p1->first)) << std::endl;
            break;
        }
        p1++;
    }
    return EXIT_SUCCESS;
}
#endif