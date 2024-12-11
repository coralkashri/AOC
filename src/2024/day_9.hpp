#ifndef ADVENTOFCODE2024_DAY_9_HPP
#define ADVENTOFCODE2024_DAY_9_HPP

#include <iostream>
#include <algorithm>
#include <iterator>
#include <map>
#include <ranges>
#include <numeric>
#include <regex>

#include "../input_source.hpp"
#include "../tools/str_number.hpp"
#include "../tools/word_delimited_by.h"
#include "../tools/matrix.hpp"
#include "../tools/point.hpp"
#include <set>
#include "../tools/range.hpp"

class solution {
public:
    solution() {
        std::for_each(std::istream_iterator<char>(INPUT_SOURCE), std::istream_iterator<char>(), [&, current_index = 0, is_free_space = false](char c) mutable {
            int required_space = c - '0';
            if (required_space) {
                if (!is_free_space) {
                    id_to_required_space.emplace_back(current_index, c - '0');
                    total_required_space += id_to_required_space.back().second;
                } else {
                    available_indexes.emplace_back(current_index, c - '0');
                }
            }

            is_free_space = !is_free_space;
            current_index += c - '0';
        });
    }

    struct first_part {};
    struct second_part {};

    void fill_available_locations(first_part) {
        auto available_indexes_it = available_indexes.begin();
        const size_t last_index = total_required_space;
        for (auto id_to_required_space_rit = id_to_required_space.rbegin(); id_to_required_space_rit != id_to_required_space.rend() && available_indexes_it != available_indexes.end() && available_indexes_it->first < last_index; ++id_to_required_space_rit) {
            size_t id = std::distance(id_to_required_space_rit, id_to_required_space.rend()) - 1;
            auto& required_space = id_to_required_space_rit->second;

            while (required_space && available_indexes_it != available_indexes.end() && available_indexes_it->first < last_index) {
                sum += available_indexes_it->first++ * id;
                if (!--available_indexes_it->second) ++available_indexes_it;
                --required_space;
            }
        }
    }
    void fill_available_locations(second_part) {
        for (auto id_to_required_space_rit = id_to_required_space.rbegin(); id_to_required_space_rit != id_to_required_space.rend() && !available_indexes.empty(); ++id_to_required_space_rit) {
            size_t id = std::distance(id_to_required_space_rit, id_to_required_space.rend()) - 1;
            auto& required_space = id_to_required_space_rit->second;

            auto available_indexes_it = available_indexes.begin();
            for (; available_indexes_it != available_indexes.end() && available_indexes_it->first < id_to_required_space_rit->first; ++available_indexes_it) {
                if (available_indexes_it->second >= required_space) {
                    available_indexes_it->second -= static_cast<long long>(required_space);
                    while (required_space--) {
                        sum += available_indexes_it->first++ * id;
                    }
                    required_space = 0;

                    if (!available_indexes_it->second) {
                        available_indexes.erase(available_indexes_it);
                    }
                    break;
                }
            }
        }
    }

    void iterate_over_all_ids() {
        size_t current_id = 0;

        auto id_to_required_space_it = id_to_required_space.begin();
        while (id_to_required_space_it != id_to_required_space.end()) {
            while (id_to_required_space_it != id_to_required_space.end() && !id_to_required_space_it->second) {
                ++id_to_required_space_it;
                ++current_id;
            }

            if (id_to_required_space_it == id_to_required_space.end()) break;

            sum += current_id * id_to_required_space_it->first++;
            --id_to_required_space_it->second;
        }
    }

    [[nodiscard]] size_t get_sum() const {
        return sum;
    }

private:
    std::vector<std::pair<size_t, long long>> available_indexes;
    std::vector<std::pair<size_t, size_t>> id_to_required_space; // vec index - id, first - starting index, second - required space
    size_t total_required_space = 0; // Part 1
    size_t sum = 0;
};

int first_part_2024_9() {
    solution s;
    s.fill_available_locations(solution::first_part{});
    s.iterate_over_all_ids();
    std::cout << s.get_sum() << "\n";

    return EXIT_SUCCESS;
}

int second_part_2024_9() {
    solution s;
    s.fill_available_locations(solution::second_part{});
    s.iterate_over_all_ids();
    std::cout << s.get_sum() << "\n";

    return EXIT_SUCCESS;
}

#endif
