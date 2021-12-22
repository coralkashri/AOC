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

void roll_dice(std::atomic<unsigned long long> *wins_count, std::vector<player> players, size_t player_turn, int left_rolls, size_t roll) {
    player &p = players.at(player_turn);
    p.place = (p.place + roll - 1) % 10 + 1;
    if (left_rolls == 0) {
        p.score += p.place;
        if (p.score >= 21) {
            wins_count[player_turn]++;
            return;
        }

        player_turn = (player_turn + 1) % 2;
        left_rolls = 3;
    }

    while (left_rolls--) {
        roll_dice(wins_count, players, player_turn, left_rolls, 1);
        roll_dice(wins_count, players, player_turn, left_rolls, 2);
        roll_dice(wins_count, players, player_turn, left_rolls, 3);
    }
}

/*struct dice_results {
    size_t first, second, third;

    size_t get_sum() const {
        return first + second + third;
    }
};

void roll_dice(std::atomic<unsigned long long> *wins_count, const std::vector<player> &players, size_t player_turn);

bool calculate_new_player_place(std::atomic<unsigned long long> *wins_count, dice_results dice, std::vector<player> players, size_t player_turn) {
    auto &p = players[player_turn];
    p.place = (p.place + dice.get_sum() - 1) % 10 + 1;
    p.score += p.place;
    if (p.score >= 21) {
        wins_count[player_turn]++;
        return true;
    }
    roll_dice(wins_count, players, (player_turn + 1) % 2);
    return false;
}

void roll_dice(std::atomic<unsigned long long> *wins_count, const std::vector<player> &players, size_t player_turn) {
    size_t iteration = 0;
    for (size_t first = 1; first <= 3; first++) {
        for (size_t second = 1; second <= 3; second++) {
            for (size_t third = 1; third <= 3; third++) {
                iteration++;
                calculate_new_player_place(wins_count, dice_results{first, second, third}, players, player_turn);
                *//*if (calculate_new_player_place(wins_count, dice_results{first, second, third}, players, player_turn)) {
                    wins_count[player_turn] += 3 * 3 * 3 - iteration;
                    return;
                }*//*
            }
        }
    }
}*/

int second_part_2021_21() {
    std::vector<player> players;
    std::atomic<unsigned long long> wins_count[2];
    wins_count[0] = 0;
    wins_count[1] = 0;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (std::string str) {
        players.emplace_back() = {(size_t)std::stol(str.substr(str.find_last_of(':') + 2)), 0};
    });

    std::thread t1([&] {roll_dice(wins_count, players, 0, 2, 1); });
    std::thread t2([&] {roll_dice(wins_count, players, 0, 2, 2); });
    std::thread t3([&] {roll_dice(wins_count, players, 0, 2, 3); });

    t1.join();
    t2.join();
    t3.join();

    std::cout << std::max(wins_count[0], wins_count[1]) << std::endl;
    std::cout << std::min(wins_count[0], wins_count[1]) << std::endl;

    return EXIT_SUCCESS;
}

#endif

// 554569744093 -> too low
// 341780601582 -> too low
// 212789142511