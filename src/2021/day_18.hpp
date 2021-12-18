#ifndef AOC_2021_18
#define AOC_2021_18

#include "../tools/base_includes.h"
#include <stack>

using namespace std::string_literals;

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

std::pair<size_t, size_t> get_inner_expr_numbers(std::string inner_expr) {
    auto left_num = std::stoi(inner_expr.substr(1, inner_expr.find(',') - 1));
    auto right_num = std::stoi(inner_expr.substr(inner_expr.find(',') + 1, inner_expr.size() - inner_expr.find(',') - 1));
    return {left_num, right_num};
}

void add_number_to_nearest_left_number(std::string &expr, size_t num, size_t pos) {
    for (int i = pos; i > 0; i--) {
        if (is_digit(expr[i])) {
            auto num_to_update_stop = i;
            while (i > 0 && is_digit(expr[--i]));
            auto num_to_update_start = i + 1;
            auto num_to_update = std::stol(expr.substr(num_to_update_start, num_to_update_stop - num_to_update_start + 1));
            num_to_update += num;
            expr.replace(num_to_update_start, num_to_update_stop - num_to_update_start + 1, std::to_string(num_to_update));
            break;
        }
    }
}

void add_number_to_nearest_right_number(std::string &expr, size_t num, size_t pos) {
    for (int i = pos; i < expr.size() - 1; i++) {
        if (is_digit(expr[i])) {
            auto num_to_update_start = i;
            while (i < expr.size() - 1 && is_digit(expr[++i]));
            auto num_to_update_stop = i - 1;
            auto num_to_update = std::stol(expr.substr(num_to_update_start, num_to_update_stop - num_to_update_start + 1));
            num_to_update += num;
            expr.replace(num_to_update_start, num_to_update_stop - num_to_update_start + 1, std::to_string(num_to_update));
            break;
        }
    }
}

void explode(std::string &str, size_t pos) {
    auto exploded_expr = str.substr(pos, str.find(']', pos) - pos);
    auto [left_num, right_num] = get_inner_expr_numbers(exploded_expr);

    auto new_begin = str.substr(0, pos);
    add_number_to_nearest_left_number(new_begin, left_num, pos - 1);

    auto new_end = str.substr(str.find(']', pos) + 1);
    add_number_to_nearest_right_number(new_end, right_num, 0);

    str = new_begin + "0" + new_end;
}

void split(std::string &str, size_t pos) {
    float num_to_split = std::stoi(""s + str[pos] + str[pos + 1]);
    size_t left_num = std::floor(num_to_split / 2.);
    size_t right_num = std::ceil(num_to_split / 2.);
    auto new_expr_start = str.substr(0, pos);
    auto new_expr_stop = str.substr(pos + 2);
    str = new_expr_start + "[" + std::to_string(left_num) + "," + std::to_string(right_num) + "]" + new_expr_stop;
}

bool check_for_split(std::string &expr) {
    for (size_t i = 0; i < expr.size() - 1; i++) {
        if (is_digit(expr[i]) && is_digit((expr[i + 1]))) {
            split(expr, i);
            return true;
        }
    }
    return false;
}

bool check_for_explode(std::string &expr) {
    size_t open_bracket_count = 0;
    for (size_t i = 0; i < expr.size(); i++) {
        open_bracket_count += expr[i] == '[';
        open_bracket_count -= expr[i] == ']';
        if (open_bracket_count == 5) {
            explode(expr, i);
            return true;
        }
    }
    return false;
}

void perform_reduce(std::string &expr) {
    bool is_action_performed = true;
    while (is_action_performed) {
        if (check_for_explode(expr)) continue;
        if (check_for_split(expr)) continue;
        is_action_performed = false;
    }
}

void perform_addition(std::string &to, std::string from) {
    if (to.empty()) to = from;
    else to = "[" + to + "," + from + "]";
    perform_reduce(to);
}

size_t calc_magnitude(size_t left, size_t right) {
    return 3 * left + 2 * right;
}

size_t calc_magnitude(std::string expr) {
    for (int i = 0; i < expr.size(); i++) {
        if (expr[i] == ']') {
            auto expr_until_this_point = expr.substr(0, i);
            size_t current_start = expr_until_this_point.find_last_of('[');
            auto current_expr = expr_until_this_point.substr(current_start);

            auto [left_num, right_num] = get_inner_expr_numbers(current_expr);

            auto mag = calc_magnitude(left_num, right_num);
            expr.replace(current_start, current_expr.size() + 1, std::to_string(mag));
            i = -1;
            continue;
        }
    }
    return std::stoll(expr);
}

int first_part_2021_18() {
    std::string expr;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [&] (std::string str) {
        perform_addition(expr, str);
    });
    std::cout << calc_magnitude(expr) << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2021_18() {
    std::vector<std::string> all_expressions;
    size_t max = 0;
    std::copy(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), std::back_inserter(all_expressions));

    for (size_t i = 0; i < all_expressions.size(); i++) {
        auto base_expr = all_expressions[i];
        for (size_t j = i + 1; j < all_expressions.size(); j++){
            auto base_expr_copy = base_expr;
            perform_addition(base_expr_copy, all_expressions[j]);
            auto mag = calc_magnitude(base_expr_copy);
            if (mag > max) max = mag;

            auto all_expressions_copy = all_expressions[j];
            base_expr_copy = base_expr;
            perform_addition(all_expressions_copy, base_expr_copy);
            mag = calc_magnitude(all_expressions_copy);
            if (mag > max) max = mag;
        }
    }

    std::cout << max << std::endl;

    return EXIT_SUCCESS;
}

#endif