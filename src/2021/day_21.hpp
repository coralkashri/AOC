#ifndef AOC_2021_21
#define AOC_2021_21

#include "../tools/base_includes.h"
#include <stack>
#include <unordered_set>

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

    std::cout << rolls_count * std::min(players.at(0).second, players.at(1).second) << std::endl;

    return EXIT_SUCCESS;
}

struct player {
    size_t place;
    size_t score;
};

void roll_dice(std::vector<unsigned long long> &wins_count, std::vector<player> players, size_t player_turn, int left_rolls, size_t roll) {
    player &p = players.at(player_turn);
    p.place = (p.place + roll - 1) % 10 + 1;
    p.score += p.place;
    if (p.score >= 21) {
        wins_count.at(player_turn)++;
        return;
    }
    while (left_rolls--) {
        roll_dice(wins_count, players, player_turn, left_rolls, 1);
        roll_dice(wins_count, players, player_turn, left_rolls, 2);
        roll_dice(wins_count, players, player_turn, left_rolls, 3);
    }

    player_turn = (player_turn + 1) % 2;
    left_rolls = 3;
    while (left_rolls--) {
        roll_dice(wins_count, players, player_turn, left_rolls, 1);
        roll_dice(wins_count, players, player_turn, left_rolls, 2);
        roll_dice(wins_count, players, player_turn, left_rolls, 3);
    }
}

int second_part_2021_21() {
    std::vector<player> players;
    std::vector<unsigned long long> wins_count(2, 0);
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (std::string str) {
        players.emplace_back() = {(size_t)std::stol(str.substr(str.find_last_of(':') + 2)), 0};
    });

    roll_dice(wins_count, players, 0, 2, 1);
    roll_dice(wins_count, players, 0, 2, 2);
    roll_dice(wins_count, players, 0, 2, 3);

    std::cout << std::max(wins_count.at(0), wins_count.at(1)) << std::endl;

    return EXIT_SUCCESS;
}

#endif