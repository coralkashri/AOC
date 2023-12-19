#ifndef AOC_2023_22
#define AOC_2023_22

#include <iostream>
#include <iterator>
#include <bitset>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

template <bool UseRecursion>
std::pair<uint8_t, std::queue<uint32_t>> play_recursion_combat(std::queue<uint32_t> player_1, std::queue<uint32_t> player_2, size_t p1_cards_count = -1, size_t p2_cards_count = -1) {
    std::set<std::tuple<std::queue<uint32_t>, std::queue<uint32_t>>> memo2;

    if (p1_cards_count != -1) {
        std::queue<size_t> deck;
        while (!player_1.empty()) {
            deck.push(player_1.front());
            player_1.pop();
        }
        for (size_t i = 0; i < p1_cards_count; ++i) {
            player_1.push(deck.front());
            deck.pop();
        }
    }

    if (p2_cards_count != -1) {
        std::queue<size_t> deck;
        while (!player_2.empty()) {
            deck.push(player_2.front());
            player_2.pop();
        }
        for (size_t i = 0; i < p2_cards_count; ++i) {
            player_2.push(deck.front());
            deck.pop();
        }
    }

    while (!player_1.empty() && !player_2.empty()) {
        if constexpr(UseRecursion) {
            if (auto it = memo2.find({player_1, player_2}); it != memo2.end()) {
                return {1, player_1};
            }
            memo2.insert({player_1, player_2});
        }

        auto p1_card = player_1.front();
        player_1.pop();
        auto p2_card = player_2.front();
        player_2.pop();

        uint8_t winner;
        if constexpr(UseRecursion) {
            if (p1_card <= player_1.size() && p2_card <= player_2.size()) {
                auto[w, winner_deck] = play_recursion_combat<UseRecursion>(player_1, player_2, p1_card, p2_card);
                winner = w;
            } else {
                winner = p1_card > p2_card ? 1 : 2;
            }
        } else {
            winner = p1_card > p2_card ? 1 : 2;
        }

        if (winner == 1) {
            player_1.push(p1_card);
            player_1.push(p2_card);
        } else {
            player_2.push(p2_card);
            player_2.push(p1_card);
        }
    }

    return !player_1.empty() ? std::pair{1, player_1} : std::pair{2, player_2};
}


size_t calculate_points(std::queue<uint32_t>& player) {
    size_t index = player.size();
    size_t res = 0;
    while (!player.empty()) {
        res += player.front() * index--;
        player.pop();
    }
    return res;
}

auto parse_input() {
    std::queue<uint32_t> player_1, player_2;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE),
                  std::istream_iterator<WordDelimitedBy<'\n'>>(),
                  [&player_1, &player_2, current_player = &player_1](std::string str) mutable {
                      if (str.empty()) return;

                      if (str[0] == 'P') {
                          if (str[7] == '1') current_player = &player_1;
                          else current_player = &player_2;
                          return;
                      }

                      current_player->push(str_number<uint32_t>(str).get_number());
                  });
    return std::pair{player_1, player_2};
}

template <bool UseRecursion>
size_t solve() {
    auto [player_1, player_2] = parse_input();
    auto [winning_player_number, winning_deck] = play_recursion_combat<UseRecursion>(player_1, player_2);
    return calculate_points(winning_deck);
}

int first_part_2020_22() {
    std::cout << solve<false>() << std::endl;
    return EXIT_SUCCESS;
}

int second_part_2020_22() {
    std::cout << solve<true>() << std::endl;
    return EXIT_SUCCESS;
}

#endif