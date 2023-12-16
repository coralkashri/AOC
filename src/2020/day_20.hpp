#ifndef AOC_2023_20
#define AOC_2023_20

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include <array>
#include <set>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

#define USE_OLD_WAY true

/*uint64_t transform_str_to_bits(std::string_view str, char one, bool should_reverse = false) {
    uint64_t number = 0;
    for (auto it = !should_reverse ? str.data() : str.data() + str.size() - 1;
         it != (!should_reverse ? str.data() + str.size() : str.data() - 1); !should_reverse ? ++it : --it) {
        number <<= 1;
        number |= *it == one;
    }
    return number;
}*/

uint64_t transform_str_to_bits(std::string_view str, char one) {
    uint64_t number = 0;
    for (char c: str) {
        number <<= 1;
        number |= c == one;
    }
    return number;
}

uint64_t transform_str_to_bits_reverse(std::string_view str, char one) {
    uint64_t number = 0;
    for (char c: str | std::views::reverse) {
        number <<= 1;
        number |= c == one;
    }
    return number;
}

class tile {
public:
    void set_title(std::string_view str) {
        number = str_number(str.substr(str.find(' ') + 1, 4)).get_number();
    }

    tile &operator+=(const std::string &str) {
        matrix.push_back(str);
        return *this;
    }

    bool operator<(const tile &ref) const {
        return shield_options < ref.shield_options;
    }

    void close_tile() {
        #if USE_OLD_WAY
        shield_options.insert(transform_str_to_bits(matrix.front(), '#'));
        shield_options.insert(transform_str_to_bits_reverse(matrix.front(), '#'));
        shield_options.insert(transform_str_to_bits(matrix.back(), '#'));
        shield_options.insert(transform_str_to_bits_reverse(matrix.back(), '#'));

        std::string right, left;
        for (std::string_view str: matrix) {
            left += str.back();
            right += str.front();
        }

        shield_options.insert(transform_str_to_bits(left, '#'));
        shield_options.insert(transform_str_to_bits_reverse(left, '#'));
        shield_options.insert(transform_str_to_bits(right, '#'));
        shield_options.insert(transform_str_to_bits_reverse(right, '#'));
        #else
        shield_options.push_back(matrix.front());
        std::string temp;
        std::ranges::copy(matrix.front() | std::views::reverse, std::back_inserter(temp));
        shield_options.push_back(temp);
        shield_options.push_back(matrix.back());
        temp.clear();
        std::ranges::copy(matrix.back() | std::views::reverse, std::back_inserter(temp));
        shield_options.push_back(temp);

        std::string right, left;
        for (std::string_view str : matrix) {
            left += str.back();
            right += str.front();
        }

        shield_options.push_back(left);
        temp.clear();
        std::ranges::copy(left | std::views::reverse, std::back_inserter(temp));
        shield_options.push_back(temp);
        shield_options.push_back(right);
        temp.clear();
        std::ranges::copy(right | std::views::reverse, std::back_inserter(temp));
        shield_options.push_back(temp);
        #endif
    }

    size_t match_count(const tile &ref) const {
        #if USE_OLD_WAY
        std::vector<size_t> intersections;
        std::set_intersection(shield_options.begin(), shield_options.end(), ref.shield_options.begin(),
                              ref.shield_options.end(), std::back_inserter(intersections));
        return intersections.size() /
               2; // If they'd match in one direction, they'd match in the exact opposite direction. Therefore, divide by 2.
        #else
        size_t count = 0;
        for (auto it = shield_options.begin(); it != shield_options.end(); ++it) {
            for (auto it_ref = ref.shield_options.begin(); it_ref != ref.shield_options.end(); ++it_ref) {
                if (*it == *it_ref) count++;
            }
        }
        return count / 2;
        #endif
    }

    void clear() {
        shield_options.clear();
        number = 0;
        matrix.clear();
    }

    size_t get_number() const {
        return number;
    };

private:
    size_t number;
    #if USE_OLD_WAY
    std::set<size_t> shield_options;
    #else
    std::vector<std::string> shield_options;
    #endif
    std::vector<std::string> matrix;

    tile *right_tile;
    tile *left_tile;
    tile *up_tile;
    tile *down_tile;
};

int first_part_2020_20() {
    std::set<tile> tiles_map;
    tile current_tile;

    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE),
                  std::istream_iterator<WordDelimitedBy<'\n'>>(),
                  [&tiles_map, &current_tile](std::string str) mutable {
                      if (str.empty()) {
                          current_tile.close_tile();
                          tiles_map.insert(current_tile);
                          current_tile.clear();
                          return;
                      }

                      if (str[0] == 'T') {
                          current_tile.set_title(str);
                          return;
                      }

                      current_tile += str;
                  });
    current_tile.close_tile();
    tiles_map.insert(current_tile);

    size_t res = 1;
    for (auto it = tiles_map.begin(); it != tiles_map.end(); ++it) {
        size_t is_edge = 0;
        for (auto it_ref = tiles_map.begin(); it_ref != tiles_map.end(); ++it_ref) {
            if (it != it_ref) {
                if (it->match_count(*it_ref)) {
                    is_edge++;
                }
            }
        }
        if (is_edge == 2) {
            std::cout << it->get_number() << "\n";
            res *= it->get_number();
        }
    }
    std::cout << res << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2020_20() {
    return EXIT_SUCCESS;
}

#endif