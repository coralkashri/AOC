#ifndef AOC_2021_4
#define AOC_2021_4

#include <iostream>
#include <bitset>
#include <numeric>
#include <vector>
#include <fstream>
#include <sstream>
#include <list>
#include "../input_source.hpp"

std::stringstream get_line_stream() {
    std::string temp_line;
    std::getline(INPUT_SOURCE, temp_line);
    return std::stringstream(temp_line);
}

template<typename T>
std::istream& operator>>(std::istream& is, std::back_insert_iterator<std::vector<T>> obj) {
    T tmp;
    is >> tmp;
    obj = tmp;
    is.ignore(1, ',');
    return is;
}

bool is_win(const std::vector<int> &board) {
    static const std::vector<std::vector<size_t>> win_indices {
            {0, 1, 2, 3, 4},
            {5, 6, 7, 8, 9},
            {10, 11, 12, 13, 14},
            {15, 16, 17, 18, 19},
            {20, 21, 22, 23, 24},
            {0, 5, 10, 15, 20},
            {1, 6, 11, 16, 21},
            {2, 7, 12, 17, 22},
            {3, 8, 13, 18, 23},
            {4, 9, 14, 19, 24},
    };
    bool res;

    for (const auto &win_combination : win_indices) {
        res = true;
        for (const auto &win_index : win_combination) {
            if (board.at(win_index) > -1) {
                res = false;
                break;
            }
        }
        if (res) break;
    }
    return res;
}

void mark_number_on_board(std::vector<int> &board, size_t number) {
    if (auto it = std::find(board.begin(), board.end(), number); it != board.end()) *it = -1;
}

size_t get_board_result(const std::vector<int> &board, size_t winning_number) {
    return std::accumulate(board.begin(), board.end(), 0, [] (int a, int b) { return b > 0 ? a + b : a; }) * winning_number;
}

size_t get_win_sum(const std::vector<size_t> &chosen_numbers, std::vector<int> &board, size_t &win_turn) {
    int skip_turns = 5;
    win_turn = 0;
    while (skip_turns--) mark_number_on_board(board, chosen_numbers.at(win_turn++));
    while (win_turn < chosen_numbers.size() && !is_win(board)) mark_number_on_board(board, chosen_numbers.at(win_turn++));
    return win_turn < chosen_numbers.size() ? get_board_result(board, chosen_numbers.at(--win_turn)) : 0;
}

bool read_board(std::vector<int> &board) {
    for (size_t line_number = 0; line_number < 5; line_number++) {
        auto current_line = get_line_stream();
        if (current_line.str().empty()) return false;
        for (size_t row_number = 0; row_number < 5; row_number++)
            current_line >> std::back_inserter(board);
    }
    return true;
}

int first_part_2021_4() {
    auto first_line = get_line_stream();
    std::vector<size_t> chosen_numbers;
    while (first_line >> std::back_inserter(chosen_numbers));

    size_t min_win_turn = chosen_numbers.size(), relevant_winning_sum; // for second part: min_win_turn = 0 <-- max_win_turn
    size_t current_win_turn, current_win_sum;
    std::vector<int> current_board;

    get_line_stream(); // Empty line
    while (read_board(current_board)) {
        current_win_sum = get_win_sum(chosen_numbers, current_board, current_win_turn);
        if (current_win_turn < min_win_turn) { // for second part: current_win_turn > max_win_turn
            min_win_turn = current_win_turn;
            relevant_winning_sum = current_win_sum;
        }
        current_board.clear();
        get_line_stream(); // Empty line
    }

    std::cout << "Win turn: " << min_win_turn << ", Winning sum: " << relevant_winning_sum << std::endl;

    return EXIT_SUCCESS;
}
#endif