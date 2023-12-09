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
    node* left; // [0] -> left, [1] -> right
    node* right; // [0] -> left, [1] -> right
};

bool reached_z(const auto& n) {
    return n->hash >= 25 && (n->hash - 25) % 26 == 0;
}

bool all_reached_z(const auto &nodes) {
    return std::ranges::all_of(nodes, [](node *n) { return reached_z(n); });
}

void all_goes_direction(auto &nodes, uint8_t direction, size_t current_steps, auto& z_steps_counters) {
    for (auto &n : nodes) {
        if (reached_z(n)) continue;
        if (!direction) n = n->left;
        else n = n->right;
        if (reached_z(n)) z_steps_counters.push_back(current_steps);
    }
}

void parse_input(std::vector<node>& graph, std::vector<node *> &start_points, std::string &operations, auto &&add_to_start_points_func) {    std::getline(INPUT_SOURCE, operations);

    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE),
                  std::istream_iterator<WordDelimitedBy<'\n'>>(),
                  [&graph, &start_points, &add_to_start_points_func](std::string str) {
                      if (str.empty()) return;

                      std::string_view str_view(str);
                      auto key = str_view.substr(0, 3);
                      auto left = str_view.substr(7, 3);
                      auto right = str_view.substr(12, 3);

                      size_t key_hash = aaa_to_zzz_string::hash(key);
                      auto &n = graph[key_hash];
                      n.hash = key_hash;
                      n.left = &graph[aaa_to_zzz_string::hash(left)];
                      n.right = &graph[aaa_to_zzz_string::hash(right)];

                      if (add_to_start_points_func(n)) {
                          start_points.push_back(&n);
                      }
                  });
}

std::vector<size_t> run_operations(std::vector<node *> &start_points, std::string &operations) {
    std::vector<size_t> z_steps_counters;
    size_t steps_counter = 0;
    while (!all_reached_z(start_points)) {
        for (char op: operations) {
            ++steps_counter;
            all_goes_direction(start_points, op != 'L', steps_counter, z_steps_counters);
            if (all_reached_z(start_points)) break;
        }
    }

    return z_steps_counters;
}

int first_part_2023_8() {
    std::vector<node> graph(26 * 26 * 26);
    std::vector<node *> start_points;
    std::string operations;

    parse_input(graph, start_points, operations, [](const node& n) { return n.hash == 0; });

    std::vector<size_t> z_steps_counters = run_operations(start_points, operations);

    std::cout << z_steps_counters[0] << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2023_8() {
    std::vector<node> graph(26 * 26 * 26);
    std::string operations;

    std::vector<node *> start_points;
    start_points.reserve(26 * 26);

    parse_input(graph, start_points, operations, [](const node& n) { return n.hash % 26 == 0; });

    std::vector<size_t> z_steps_counters = run_operations(start_points, operations);

    std::cout << lcm(z_steps_counters) << std::endl;

    return EXIT_SUCCESS;
}

#endif