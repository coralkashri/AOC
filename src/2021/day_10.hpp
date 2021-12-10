#ifndef AOC_2021_10
#define AOC_2021_10

#include <iostream>
#include <iterator>
#include <ranges>
#include <vector>
#include <list>
#include <chrono>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>
#include <ranges>
#include <numeric>
#include <cmath>
#include "../input_source.hpp"

enum corruption_type {
    CORRECT,
    CORRUPTED,
    INCOMPLETE
};

bool is_opener(char a) {
    return a == '{' || a == '[' || a == '<' || a == '(';
}

char get_corresponded_close(char open) {
    static std::unordered_map<char, char> open_to_close_map{{'(', ')'}, {'[', ']'}, {'{', '}'}, {'<', '>'}};
    return open_to_close_map.at(open);
}

std::string examine_expression(std::string expression, char &corrupted_data) {
    std::string res;
    corrupted_data = '-';
    for (auto c : expression) {
        if (is_opener(c)) res += c;
        else {
            if (get_corresponded_close(res.back()) == c)
                res = res.substr(0, res.size() - 1);
            else {
                corrupted_data = c;
                break;
            }
        }
    }
    return res;
}

std::pair<corruption_type, std::string> get_expression_analysis_type(std::string &expression, char &corrupted_data) {
    auto expression_analysis = examine_expression(expression, corrupted_data);
    if (corrupted_data != '-') return {CORRUPTED, expression_analysis};
    if (expression_analysis.empty()) return {CORRECT, expression_analysis};
    return {INCOMPLETE, expression_analysis};
}

size_t calculate_value(char close) {
    static std::unordered_map<char, size_t> char_to_val_map {{')', 3}, {']', 57}, {'}', 1197}, {'>', 25137}};
    return char_to_val_map.at(close);
}

int first_part_2021_10() {
    std::vector<char> illegal_characters;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [&] (auto line) {
        char illegal_character;
        if (get_expression_analysis_type(line, illegal_character).first == CORRUPTED)
            illegal_characters.template emplace_back(illegal_character);
    });

    std::cout << "Result: " << std::transform_reduce(illegal_characters.begin(), illegal_characters.end(), 0, std::plus<>{}, [] (auto close) { return calculate_value(close); }) << std::endl;

    return EXIT_SUCCESS;
}

long long get_close_val(char close) {
    static std::unordered_map<char, long long> char_to_val_map{{')', 1}, {']', 2}, {'}', 3}, {'>', 4}};
    return char_to_val_map.at(close);
}

long long compute_incomplete_completed_line_value(std::string missing_closes) {
    long long score = 0;
    for (const auto &missing_close : std::ranges::reverse_view(missing_closes)) {
        score = score * 5 + get_close_val(get_corresponded_close(missing_close));
    }
    return score;
}

int second_part_2021_10() {
    std::set<long long> incomplete_lines_values;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [&] (auto line) {
        char illegal_character;
        if (auto analysis_res = get_expression_analysis_type(line, illegal_character); analysis_res.first == INCOMPLETE) {
            incomplete_lines_values.insert(compute_incomplete_completed_line_value(analysis_res.second));
        }
    });

    std::cout << "Result: " << *std::next(incomplete_lines_values.begin(), incomplete_lines_values.size() / 2)  << std::endl;

    return EXIT_SUCCESS;
}

#endif