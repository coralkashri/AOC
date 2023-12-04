#ifndef AOC_2023_4
#define AOC_2023_4

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

class card {
public:
    explicit card(std::string_view str) {
        str = str.substr(str.find(':') + 2);
        std::string_view winning_section = str.substr(0, str.find('|'));
        std::string_view contained_section = str.substr(str.find('|') + 2);
        str_number current_number;

        for (char i : winning_section) {
            if (i == ' ') {
                if (current_number.is_number_ready()) {
                    winning_numbers.insert(current_number.get_number());
                    current_number.reset();
                }
                continue;
            }
            current_number += i;
        }

        for (char i : contained_section) {
            if (i == ' ') {
                if (current_number.is_number_ready()) {
                    contained_numbers.insert(current_number.get_number());
                    current_number.reset();
                }
                continue;
            }
            current_number += i;
        }

        contained_numbers.insert(current_number.get_number());
    }

    size_t intersections_count() {
        std::vector<uint32_t> intersections;
        intersections.reserve(std::min(winning_numbers.size(), contained_numbers.size()));
        std::set_intersection(contained_numbers.begin(), contained_numbers.end(), winning_numbers.begin(),  winning_numbers.end(), std::back_inserter(intersections));
        return intersections.size();
    }

    size_t compute_points() {
        size_t intersections = intersections_count();
        return intersections ? static_cast<size_t>(std::pow(2.0, intersections - 1)) : 0;
    }

private:
    std::set<uint32_t> winning_numbers;
    std::set<uint32_t> contained_numbers;
};

int first_part_2023_4() {
    std::cout << std::accumulate(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE),
                                 std::istream_iterator<WordDelimitedBy<'\n'>>(),
                                 0ull, [](size_t res, const std::string &str) {
                card c(str);
                return res + c.compute_points();
            }) << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2023_4() {
    std::array<size_t, 250> cards{};
    std::fill(cards.begin(),  cards.end(), 0);
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE),
                                 std::istream_iterator<WordDelimitedBy<'\n'>>(),
                                 [&cards, id = 1ull](const std::string &str) mutable {
                card c(str);
                ++cards[id];
                size_t points = c.intersections_count();
                for (size_t i = 0; i < points; ++i) {
                    cards[id + i + 1] += cards[id];
                }
                ++id;
            });

    std::cout << std::reduce(cards.begin(),  cards.end(), 0ull) << std::endl;

    return EXIT_SUCCESS;
}

#endif