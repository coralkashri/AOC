#ifndef ADVENTOFCODE2024_DAY_15_HPP
#define ADVENTOFCODE2024_DAY_15_HPP

#include <iostream>
#include <algorithm>
#include <iterator>
#include <map>
#include <ranges>
#include <numeric>
#include <regex>
#include <cmath>

#include "../input_source.hpp"
#include "../tools/str_number.hpp"
#include "../tools/word_delimited_by.h"
#include "../tools/matrix.hpp"
#include "../tools/point.hpp"
#include <set>
#include <thread>
#include "../tools/range.hpp"

#include "../tools/linear_algebra/all.hpp"

class map {
public:
    void insert_line(std::string_view str) {
        data.insert_line(str);
        if (auto s = str.find('@'); s != std::string::npos) {
            location = {static_cast<long long>(s), static_cast<long long>(data.size_y() - 1)};
        }
    }

    void apply_action(char action) {
        switch (action) {
            case '>':
                move_right();
                break;
            case '<':
                move_left();
                break;
            case '^':
                move_up();
                break;
            case 'v':
                move_down();
                break;
            default:
                break;
        }
    }

    void print() {
        data.print();
    }

    size_t get_value() {
        size_t value = 0;
        data.for_each([&value](char c, long long x, long long y) {
            if (c == 'O') {
                value += 100 * y + x;
            }
        });
        return value;
    }

private:
    aoc_tools::matrix<char> data;
    point_xd<2> location{-1, -1};

    void move_right() {
        long long steps = 1;
        auto sign = data[location[0] + steps, location[1]];
        while (sign != '.' && sign != '#') {
            sign = data[location[0] + ++steps, location[1]];
        }

        if (sign == '#') return;

        if (steps > 1) {
            std::swap(data[location[0] + 1, location[1]], data[location[0] + steps, location[1]]);
        }

        std::swap(data[location], data[location[0] + 1, location[1]]);
        ++location[0];
    }

    void move_left() {
        long long steps = -1;
        auto sign = data[location[0] + steps, location[1]];
        while (sign != '.' && sign != '#') {
            sign = data[location[0] + --steps, location[1]];
        }

        if (sign == '#') return;

        if (steps < -1) {
            std::swap(data[location[0] - 1, location[1]], data[location[0] + steps, location[1]]);
        }

        std::swap(data[location], data[location[0] - 1, location[1]]);
        --location[0];
    }

    void move_up() {
        long long steps = -1;
        auto sign = data[location[0], location[1] + steps];
        while (sign != '.' && sign != '#') {
            sign = data[location[0], location[1] + --steps];
        }

        if (sign == '#') return;

        if (steps < -1) {
            std::swap(data[location[0], location[1] - 1], data[location[0], location[1] + steps]);
        }

        std::swap(data[location], data[location[0], location[1] - 1]);
        --location[1];
    }

    void move_down() {
        long long steps = 1;
        auto sign = data[location[0], location[1] + steps];
        while (sign != '.' && sign != '#') {
            sign = data[location[0], location[1] + ++steps];
        }

        if (sign == '#') return;

        if (steps > 1) {
            std::swap(data[location[0], location[1] + 1], data[location[0], location[1] + steps]);
        }

        std::swap(data[location], data[location[0], location[1] + 1]);
        ++location[1];
    }
};

int first_part_2024_15() {
    map m;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&m](std::string str) {
        if (str.empty()) return;
        if (str.contains('#')) {
            m.insert_line(str);
            return;
        }
        for (auto c : str) {
            m.apply_action(c);
        }
    });

    std::cout << m.get_value() << "\n";

    return EXIT_SUCCESS;
}

class map_extended {
public:
    void insert_line(std::string_view str) {
        std::string new_str;
        long long index = 0;
        for (char c : str) {
            switch (c) {
                case '@':
                    location = {index, static_cast<long long>(data.size_y())};
                    new_str += "@.";
                    break;
                case '#':
                    new_str += "##";
                    break;
                case 'O':
                    new_str += "[]";
                    break;
                case '.':
                    new_str += "..";
                    break;
                default:
                    exit(1);
            }
            index += 2;
        }

        data.insert_line(new_str);
    }

    enum direction {
        above = -1,
        below = 1
    };

    void apply_action(char action) {
        switch (action) {
            case '>':
                move_right();
                break;
            case '<':
                move_left();
                break;
            case '^':
                move_vertically<above>();
                break;
            case 'v':
                move_vertically<below>();
                break;
            default:
                break;
        }
    }

    void print() {
        data.print();
    }

    size_t get_value() {
        size_t value = 0;
        data.for_each([&value](char c, long long x, long long y) {
            if (c == '[') {
                value += 100 * y + x;
            }
        });
        return value;
    }

private:
    aoc_tools::matrix<char> data;
    point_xd<2> location{-1, -1};

    void move_right() {
        long long steps = 1;
        auto sign = data[location[0] + steps, location[1]];
        while (sign != '.' && sign != '#') {
            sign = data[location[0] + ++steps, location[1]];
        }

        if (sign == '#') return;

        if (steps > 1) {
            data[location[0] + 1, location[1]] = '.';
            --steps;
            for (long long i = 1; i <= steps; ++i) {
                data[location[0] + 1 + i, location[1]] = i % 2 == 1 ? '[' : ']';
            }
        }

        std::swap(data[location], data[location[0] + 1, location[1]]);
        ++location[0];
    }

    void move_left() {
        long long steps = -1;
        auto sign = data[location[0] + steps, location[1]];
        while (sign != '.' && sign != '#') {
            sign = data[location[0] + --steps, location[1]];
        }

        if (sign == '#') return;

        if (steps < -1) {
            data[location[0] - 1, location[1]] = '.';
            ++steps;
            for (long long i = steps; i < 0; ++i) {
                data[location[0] - 1 + i, location[1]] = i % 2 == 0 ? '[' : ']';
            }
        }

        std::swap(data[location], data[location[0] - 1, location[1]]);
        --location[0];
    }

    template <direction d>
    std::pair<point_xd<2>, point_xd<2>> get_block(point_xd<2> position) {
        if (data[position[0], position[1] + d] == '[') {
            return {{position[0], position[1] + d}, {position[0] + 1, position[1] + d}};
        }
        if (data[position[0], position[1] + d] == ']') {
            return {{position[0] - 1, position[1] + d}, {position[0], position[1] + d}};
        }
        return {};
    }

    template <direction d>
    bool can_move_block(std::pair<point_xd<2>, point_xd<2>> block) {
        auto [block_start, block_stop] = block;
        if (data[block_start[0], block_start[1] + d] == '#' || data[block_stop[0], block_stop[1] + d] == '#') return false;
        if (data[block_start[0], block_start[1] + d] == '.' && data[block_stop[0], block_stop[1] + d] == '.') return true;

        bool can_move = true;

        if (data[block_start[0], block_start[1] + d] != '.') {
            can_move = can_move_block<d>(get_block<d>(block_start));
        }

        if (data[block_start[0], block_start[1] + d] != '[') { // Which means '.' or ']' and therefore this one can be [
            if (data[block_stop[0], block_stop[1] + d] != '.') {
                can_move = can_move && can_move_block<d>(get_block<d>(block_stop));
            }
        }

        return can_move;
    }

    template <direction d>
    void move_block(std::pair<point_xd<2>, point_xd<2>> block) {
        auto [block_start, block_stop] = block;

        if (data[block_start[0], block_start[1] + d] == '.' && data[block_stop[0], block_stop[1] + d] == '.') {
            data[block_start[0], block_start[1] + d] = '[';
            data[block_stop[0], block_stop[1] + d] = ']';
            data[block_start] = '.';
            data[block_stop] = '.';
            return;
        }

        if (data[block_start[0], block_start[1] + d] != '.') {
            move_block<d>(get_block<d>(block_start));
        }

        if (data[block_start[0], block_start[1] + d] != '[') { // Which means '.' or ']' and therefore this one can be [
            if (data[block_stop[0], block_stop[1] + d] != '.') {
                move_block<d>(get_block<d>(block_stop));
            }
        }

        data[block_start[0], block_start[1] + d] = '[';
        data[block_stop[0], block_stop[1] + d] = ']';
        data[block_start] = '.';
        data[block_stop] = '.';
    }

    template <direction d>
    void move_vertically() {
        auto sign = data[location[0], location[1] + d];
        if (sign != '.' && sign != '#') {
            if (!can_move_block<d>(get_block<d>(location))) {
                return;
            }
        }

        if (sign == '#') return;

        if (sign != '.') {
            move_block<d>(get_block<d>(location));
        }

        std::swap(data[location], data[location[0], location[1] + d]);
        location[1] += d;
    }
};

int second_part_2024_15() {
    map_extended m;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&m](std::string str) {
        if (str.empty()) return;
        if (str.contains('#')) {
            m.insert_line(str);
            return;
        }

        for (auto c : str) {
            m.apply_action(c);
        }
    });

    std::cout << m.get_value() << "\n";

    return EXIT_SUCCESS;
}

#endif
