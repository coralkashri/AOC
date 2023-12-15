#ifndef AOC_2023_15
#define AOC_2023_15

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include <array>
#include <set>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

int first_part_2023_15() {
    std::cout << std::transform_reduce(std::istream_iterator<WordDelimitedBy<','>>(INPUT_SOURCE),
                                       std::istream_iterator<WordDelimitedBy<','>>(),
                                       0ull, std::plus<>(), [](std::string str) {
                size_t res = 0;
                for (char c: str) {
                    res += c;
                    res *= 17;
                    res %= 256;
                }
                return res;
            }) << std::endl;
    return EXIT_SUCCESS;
}

int second_part_2023_15() {
    std::array<std::vector<std::pair<std::string, uint8_t>>, 256> boxes{};

    std::cout << std::transform_reduce(std::istream_iterator<WordDelimitedBy<','>>(INPUT_SOURCE),
                                       std::istream_iterator<WordDelimitedBy<','>>(),
                                       0ll, std::plus<>(), [&boxes](std::string str) -> int64_t {
                std::string_view str_view(str);
                std::string_view label;
                int64_t hash = 0;
                size_t operation_char_index = 0;
                str_number new_len;
                for (char c: str_view) {
                    if (c == '-' || c == '=') break;
                    hash += c;
                    hash *= 17;
                    hash %= 256;
                    ++operation_char_index;
                }
                label = str_view.substr(0, operation_char_index);

                bool should_update = str_view[operation_char_index] == '=';
                if (should_update) {
                    new_len = str_view.substr(operation_char_index + 1);
                }

                auto &box = boxes[hash];
                int64_t value_to_add = 0;
                int64_t index = 1;
                auto it = box.begin();
                for (; it != box.end(); ++it) {
                    if (it->first == label) {
                        value_to_add = (hash + 1) * index;
                        if (should_update) {
                            value_to_add *= (new_len.get_number() - it->second);
                            it->second = new_len.get_number();
                            return value_to_add;
                        } else {
                            value_to_add *= -it->second;
                            it = box.erase(it);
                            break;
                        }
                    }
                    ++index;
                }

                if (!should_update) {
                    for (; it != box.end(); ++it) {
                        value_to_add -= (hash + 1) * it->second;
                    }
                    return value_to_add;
                }

                box.emplace_back(label, new_len.get_number());
                return (hash + 1) * index * new_len.get_number();
            }) << std::endl;

    return EXIT_SUCCESS;
}

#endif