#ifndef AOC_2021_21
#define AOC_2021_21

#include "../tools/base_includes.h"
#include <stack>
#include <unordered_set>
#include <atomic>
#include <thread>

int first_part_2021_21() {
    std::vector<std::pair<size_t, size_t>> players;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (std::string str) {
        players.emplace_back() = std::pair{std::stoi(str.substr(str.find_last_of(':') + 2)), 0};
    });

    bool is_1000_reached = false;
    size_t last_roll = 0;
    size_t rolls_count = 0;
    while (!is_1000_reached) {
        for (auto &player : players) {
            size_t current_rol_sum = last_roll + 1 + last_roll + 2 + last_roll + 3;
            last_roll += 3;
            rolls_count += 3;
            player.first = (player.first + current_rol_sum - 1) % 10 + 1;
            player.second += player.first;
            is_1000_reached = player.second >= 1000;
            if (is_1000_reached) break;
        }
    }

    std::cout << rolls_count << " " << players.at(0).second << " " << players.at(1).second << std::endl;
    std::cout << rolls_count * std::min(players.at(0).second, players.at(1).second) << std::endl;

    return EXIT_SUCCESS;
}

struct player {
    size_t place;
    size_t score;
};

void roll_dice(unsigned long long *wins_count, std::vector<player> players, size_t player_turn, unsigned long weight = 1);

void calculate_new_player_place(unsigned long long *wins_count, std::pair<size_t, size_t> dice, std::vector<player> players, size_t player_turn, unsigned long weight) {
    auto &p = players[player_turn];
    p.place = (p.place + dice.first - 1) % 10 + 1;
    p.score += p.place;
    if (p.score >= 21) {
        wins_count[player_turn] += weight;
        return;
    }
    roll_dice(wins_count, players, (player_turn + 1) % 2, weight);
}

void roll_dice(unsigned long long *wins_count, std::vector<player> players, size_t player_turn, unsigned long weight) {
    std::vector<std::pair<size_t, size_t>> roll_sum_possibilities = {
            {3, 1}, // 1, 1, 1
            {4, 3}, // 1, 1, 2 | 1, 2, 1 | 2, 1, 1
            {5, 6}, // 1, 1, 3 | 1, 2, 2 | 1, 3, 1 | 2, 1, 2 | 2, 2, 1 | 3, 1, 1
            {6, 7}, // 1, 2, 3 | 1, 3, 2 | 2, 1, 3 | 2, 2, 2 | 2, 3, 1 | 3, 1, 2 | 3, 2, 1
            {7, 6}, // 1, 3, 3 | 2, 2, 3 | 2, 3, 2 | 3, 1, 3 | 3, 2, 2 | 3, 3, 1
            {8, 3}, // 2, 3, 3 | 3, 2, 3 | 3, 3, 2
            {9, 1}, // 3, 3, 3
    };
    for (auto &possibility : roll_sum_possibilities) {
        calculate_new_player_place(wins_count, possibility, players, player_turn, weight * possibility.second);
    }
}

int second_part_2021_21() {
    std::vector<player> players;
    unsigned long long wins_count[2];
    wins_count[0] = 0;
    wins_count[1] = 0;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (std::string str) {
        players.emplace_back() = {(size_t)std::stol(str.substr(str.find_last_of(':') + 2)), 0};
    });

    roll_dice(wins_count, players, 0);

    std::cout << std::max(wins_count[0], wins_count[1]) << std::endl;
    std::cout << std::min(wins_count[0], wins_count[1]) << std::endl;

    return EXIT_SUCCESS;
}

#endif