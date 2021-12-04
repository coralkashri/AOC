#ifndef AOC_2020_2
#define AOC_2020_2

#include <iostream>
#include <unordered_map>
#include <set>
#include <algorithm>
#include "../input_source.hpp"

int first_part_2020_2() {
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

#endif
