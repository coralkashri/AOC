#ifndef AOC_2023_7
#define AOC_2023_7

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include <array>
#include <set>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

template<bool UseJocker = false>
class hand {
private:
    enum special_card_types {
        T = 9 - !UseJocker,
        J = UseJocker ? 0 : 9,
        Q = 10,
        K = 11,
        A = 12,

        TypesCount
    };

    class cards_map {
    public:
        explicit cards_map(std::string_view cards) : cards_str(cards) {
            std::fill_n(cards_amount.begin(), cards_amount.size(), 0);
        }

        uint8_t &operator[](char card) {
            return cards_amount[std::isdigit(card) ? card - '0' - 2 + UseJocker : convert_card_to_type(card)];
        }

        std::string_view get_all_cards() {
            return cards_str;
        }

        uint64_t get_card_value() const {
            size_t value = 0;
            for (char c: cards_str) {
                value *= 100;
                if (std::isdigit(c)) {
                    value += c - '0' - 2 + UseJocker;
                } else {
                    value += convert_card_to_type(c);
                }
            }

            return value;
        }

    private:
        std::string cards_str;
        std::array<uint8_t, TypesCount> cards_amount;

        special_card_types convert_card_to_type(char card) const {
            switch (card) {
                case 'T':
                    return T;
                case 'J':
                    return J;
                case 'Q':
                    return Q;
                case 'K':
                    return K;
                case 'A':
                    return A;
                default:
                    exit(1);
            }
        }
    };

public:
    explicit hand(std::string_view player) : cards(player.substr(0, 5)) {
        str_number<uint64_t> str_bid;
        for (char c: player.substr(6)) {
            str_bid += c;
        }
        bid = str_bid.get_number();
        analyze_hand_kind(cards.get_all_cards());
    }

    bool operator<(const hand &other) const {
        return hand_strength < other.hand_strength ||
               (hand_strength == other.hand_strength && cards.get_card_value() < other.cards.get_card_value());
    }

    uint64_t get_bid() const {
        return bid;
    }

private:
    uint64_t bid;
    uint8_t hand_strength = 0;
    cards_map cards;

    void analyze_hand_kind(std::string_view cards_str) {
        size_t j_counter = 0;
        for (char c: cards_str) {
            if constexpr(UseJocker) {
                if (c == 'J') {
                    j_counter++;
                    continue;
                }
            }

            uint8_t new_amount = ++cards[c];
            if (new_amount == 2 || new_amount == 5) hand_strength++;
            else if (new_amount == 3 || new_amount == 4) hand_strength += 2;
        }

        if constexpr(!UseJocker)
            return;

        /* 1 Joker calculation:
        OnePair = 1, TwoPairs = 2, ThreeOfKind = 3, FullHouse = 4, FourOfKind = 5, FiveOfKind = 6
        0 -> 1 | 1 -> 3 | 2 -> 4 | 3 -> 5 | 4 -> 5 | 5 -> 6
        */

        std::array<uint8_t, 7> transform_with_j = {1, 3, 4, 5, 5, 6, 6};
        for (size_t i = 0; i < j_counter; ++i) {
            hand_strength = transform_with_j[hand_strength];
        }
    }
};

template<bool UseJoker>
void analyze_and_solve() {
    std::set<hand<UseJoker>> hands;
    std::transform(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE),
                   std::istream_iterator<WordDelimitedBy<'\n'>>(), std::inserter(hands, hands.begin()),
                   [](std::string str) {
                       return hand<UseJoker>(str);
                   });

    size_t sum = 0;
    size_t index = 1;
    for (auto &h: hands) {
        sum += h.get_bid() * index++;
    }
    std::cout << sum << std::endl;
}

int first_part_2023_7() {
    analyze_and_solve<false>();
    return EXIT_SUCCESS;
}

int second_part_2023_7() {
    analyze_and_solve<true>();
    return EXIT_SUCCESS;
}

#endif