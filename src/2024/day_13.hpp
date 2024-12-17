#ifndef ADVENTOFCODE2024_DAY_13_HPP
#define ADVENTOFCODE2024_DAY_13_HPP

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
#include "../tools/range.hpp"

#include "../tools/linear_algebra/all.hpp"

struct traveler {
    point_xd<2> location = {0, 0};
    size_t cost = 0;
    size_t a_press_times = 0, b_press_times = 0;

    bool operator<(const traveler &ref) const {
        return std::tie(cost, location, a_press_times, b_press_times) < std::tie(ref.cost, ref.location, ref.a_press_times, ref.b_press_times);
    }

    bool operator>(const traveler &ref) const {
        return cost > ref.cost;
    }
};

size_t calculate_min_cost_required_for_win(point_xd<2> button_a_config, point_xd<2> button_b_config, point_xd<2> prize) {
    std::priority_queue<traveler, std::vector<traveler>, std::greater<>> next_to_try;
    next_to_try.push({0});
    std::set<traveler> tries;
    while (!next_to_try.empty()) {
        auto state = next_to_try.top();
        next_to_try.pop();

        if (state.location[0] > prize[0] || state.location[1] > prize[1]) continue;

        if (state.location != point_xd<2>{0, 0} &&
        prize[0] % state.location[0] == 0 && state.location[1] * prize[0] / state.location[0] == prize[1])
            return state.cost;

        if (tries.contains(state)) continue;
        tries.insert(state);

        if (state.a_press_times < 100) {
            next_to_try.push({.location = state.location + button_a_config,
                              .cost = state.cost + 3,
                                     .a_press_times = state.a_press_times + 1,
                                     .b_press_times = state.b_press_times});
        }

        if (state.b_press_times < 100) {
            next_to_try.push({.location = state.location + button_b_config,
                                     .cost = state.cost + 1,
                                     .a_press_times = state.a_press_times,
                                     .b_press_times = state.b_press_times + 1});
        }
    }
    return {0};
}

int first_part_2024_13() {
    std::cout << std::transform_reduce(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), 0uz, std::plus{}, [button_a = point_xd<2>(), button_b = point_xd<2>()](std::string str) mutable {
        if (str.empty()) return 0uz;
        std::string_view strv = str;

        if (strv.contains("Button")) {
            auto x_equal = strv.find('+');
            auto y_equal = strv.rfind('+');
            auto x = str_number(strv.substr(x_equal + 1, strv.find(',') - x_equal - 1)).get_number();
            auto y = str_number(strv.substr(y_equal + 1)).get_number();
            if (strv.contains('A')) {
                button_a = {x, y};
            } else {
                button_b = {x, y};
            }
            return 0uz;
        }

        // Prize
        auto x_equal = strv.find('=');
        auto y_equal = strv.rfind('=');

        auto x = str_number(strv.substr(x_equal + 1, strv.find(',') - x_equal - 1)).get_number();
        auto y = str_number(strv.substr(y_equal + 1)).get_number();

        return calculate_min_cost_required_for_win(button_a, button_b, {x, y});
    }) << "\n";
    return EXIT_SUCCESS;
}

int second_part_2024_13() {
    std::cout << std::transform_reduce(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), 0uz, std::plus{}, [button_a = point_xd<2>(), button_b = point_xd<2>()](std::string str) mutable {
        if (str.empty()) return 0uz;
        std::string_view strv = str;

        if (strv.contains("Button")) {
            auto x_equal = strv.find('+');
            auto y_equal = strv.rfind('+');
            auto x = str_number(strv.substr(x_equal + 1, strv.find(',') - x_equal - 1)).get_number();
            auto y = str_number(strv.substr(y_equal + 1)).get_number();
            if (strv.contains('A')) {
                button_a = {x, y};
            } else {
                button_b = {x, y};
            }
            return 0uz;
        }

        // Prize
        auto x_equal = strv.find('=');
        auto y_equal = strv.rfind('=');

        auto x_result = str_number(strv.substr(x_equal + 1, strv.find(',') - x_equal - 1)).get_number() + 10000000000000;
        auto y_result = str_number(strv.substr(y_equal + 1)).get_number() + 10000000000000;

        auto x1 = button_a[0];
        auto x2 = button_b[0];

        auto y1 = button_a[1];
        auto y2 = button_b[1];

        std::vector<LinearAlgebra::hyperplane> hyperplanes;
        hyperplanes.emplace_back(LinearAlgebra::vector({static_cast<double>(x1), static_cast<double>(x2)}), x_result);
        hyperplanes.emplace_back(LinearAlgebra::vector({static_cast<double>(y1), static_cast<double>(y2)}), y_result);
        auto s = LinearAlgebra::linear_system(hyperplanes);
        auto res = s.compute_ge_solution();

        long long test_res_a = std::round(res.res[0]);
        long long test_res_b = std::round(res.res[1]);

        if (point_xd<2>{test_res_a * button_a[0] + test_res_b * button_b[0], test_res_a * button_a[1] + test_res_b * button_b[1]} == point_xd<2>{x_result, y_result}) {
            return static_cast<size_t>(test_res_a) * 3 + static_cast<size_t>(test_res_b);
        }

        return 0uz;
    }) << "\n";
    return EXIT_SUCCESS;
}

#endif
