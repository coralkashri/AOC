#ifndef AOC_2023_8
#define AOC_2023_8

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include <array>
#include <set>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

struct node {
    size_t hash;
    node *right;
    node *left;
};

uint64_t get_str_location(std::string_view str) {
    uint64_t res = 0;
    int8_t location = 2;
    for (char c: str) {
        res += (c - 'A') * std::pow(26, location--);
    }
    return res;
}

struct aaa_to_zzz_string {
    std::string stringify;

    aaa_to_zzz_string &operator++() {
        if (++stringify[2] == 'Z' + 1) {
            stringify[2] = 'A';
            ++stringify[1];
        }

        if (stringify[1] == 'Z' + 1) {
            stringify[1] = 'A';
            ++stringify[0];
        }

        if (stringify[0] == 'Z' + 1) {
            exit(1);
        }

        return *this;
    }
};

void print_all_numbers_aaa_to_zzz() {
    aaa_to_zzz_string my_str{"AAA"};
    for (size_t i = 0; i < 26 * 26 * 26; ++i) {
        std::cout << my_str.stringify << " " << get_str_location(my_str.stringify) << " " << i << std::endl;
        if (i < 26 * 26 * 26 - 1) {
            ++my_str;
        }
    }
}

int first_part_2023_8() {
    std::vector<node> graph(26 * 26 * 26);
    std::string operations;
    std::getline(INPUT_SOURCE, operations);

    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE),
                  std::istream_iterator<WordDelimitedBy<'\n'>>(),
                  [&graph](std::string str) {
                      if (str.empty()) return;

                      std::string_view str_view(str);
                      auto key = str_view.substr(0, 3);
                      auto left = str_view.substr(7, 3);
                      auto right = str_view.substr(12, 3);

                      size_t key_hash = get_str_location(key);
                      auto &n = graph[key_hash];
                      n.hash = key_hash;
                      n.left = &graph[get_str_location(left)];
                      n.right = &graph[get_str_location(right)];
                  });

    node *current = &graph[0];
    size_t steps_counter = 0;
    while (current != &graph[26 * 26 * 26 - 1]) {
        for (char op: operations) {
            ++steps_counter;
            if (op == 'L') current = current->left;
            else current = current->right;
            if (current == &graph[26 * 26 * 26 - 1]) break;
        }
    }

    std::cout << steps_counter << std::endl;

    return EXIT_SUCCESS;
}

constexpr auto lcm(const std::vector<size_t>& numbers)
{
    size_t result = numbers[0];
    for (size_t n : numbers) {
        result = std::lcm(result, n);
    }
    return result;
}

int second_part_2023_8() {
    std::vector<node> graph(26 * 26 * 26);
    std::string operations;
    std::getline(INPUT_SOURCE, operations);

    std::vector<node *> currents;
    currents.reserve(26 * 26);

    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE),
                  std::istream_iterator<WordDelimitedBy<'\n'>>(),
                  [&graph, &currents](std::string str) {
                      if (str.empty()) return;

                      std::string_view str_view(str);
                      auto key = str_view.substr(0, 3);
                      auto left = str_view.substr(7, 3);
                      auto right = str_view.substr(12, 3);

                      size_t key_hash = get_str_location(key);
                      auto &n = graph[key_hash];
                      n.hash = key_hash;
                      n.left = &graph[get_str_location(left)];
                      n.right = &graph[get_str_location(right)];

                      if (key_hash % 26 == 0) currents.push_back(&n);
                  });

    auto is_reached_z = [](const auto& n) {
        return n->hash >= 25 && (n->hash - 25) % 26 == 0;
    };

    auto all_reached_z = [&is_reached_z](const auto &nodes) {
        return std::ranges::all_of(nodes, [&is_reached_z](node *n) {
                                       return is_reached_z(n);
                                   }
        );
    };

    auto all_goes_left = [&is_reached_z](auto &nodes, size_t current_steps, auto& z_steps_counters) {
        for (auto &n: nodes) {
            if (is_reached_z(n)) continue;
            n = n->left;
            if (is_reached_z(n)) z_steps_counters.push_back(current_steps);
        }
    };

    auto all_goes_right = [&is_reached_z](auto &nodes, size_t current_steps, auto& z_steps_counters) {
        for (auto &n: nodes) {
            if (is_reached_z(n)) continue;
            n = n->right;
            if (is_reached_z(n)) z_steps_counters.push_back(current_steps);
        }
    };

    std::vector<size_t> z_steps_counters;

    size_t steps_counter = 0;
    while (!all_reached_z(currents)) {
        for (char op: operations) {
            ++steps_counter;
            if (op == 'L') all_goes_left(currents, steps_counter, z_steps_counters);
            else all_goes_right(currents, steps_counter, z_steps_counters);
            if (all_reached_z(currents)) break;
        }
    }

    std::cout << lcm(z_steps_counters) << std::endl;

    return EXIT_SUCCESS;
}

#endif