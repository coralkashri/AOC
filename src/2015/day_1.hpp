#ifndef AOC_2015_1
#define AOC_2015_1

#include "../tools/base_includes.h"
#include <stack>
#include <unordered_set>
#include <atomic>
#include <thread>

int first_part_2015_1() {
    std::cout << std::transform_reduce(std::istream_iterator<char>(INPUT_SOURCE), std::istream_iterator<char>(), 0, std::plus<>{}, [] (char c) {
        return c == '(' ? 1 : -1;
    }) << std::endl;
    return EXIT_SUCCESS;
}

int second_part_2015_1() {
    size_t position = 0;
    std::find_if(std::istream_iterator<char>(INPUT_SOURCE), std::istream_iterator<char>(), [&position, current_sum = 0] (char c) mutable {
        position++;
        current_sum += c == '(' ? 1 : -1;
        return current_sum < 0;
    });
    std::cout << position << std::endl;
    return EXIT_SUCCESS;
}

#endif