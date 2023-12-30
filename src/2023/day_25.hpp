#ifndef AOC_2023_25
#define AOC_2023_25

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include <array>
#include <set>
#include <queue>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

struct node {
    std::string name;
    std::vector<node*> connections;
    std::vector<node*> temp_connections;
};

struct connection_strength {
    size_t strength = 0;
    std::string side1, side2;

    bool operator<(const connection_strength& ref) const {
        return strength < ref.strength;
    }
};

struct state {
    const node* n = nullptr;
    size_t length = 0;

    bool operator>(const state& ref) const {
        return length > ref.length;
    }
};

size_t find_shortest_path(const node& start, const node& destination) {
    std::priority_queue<state, std::vector<state>, std::greater<>> next_to_visit;
    std::set<const node*> visited;

    next_to_visit.push({&start, 0});

    while (!next_to_visit.empty()) {
        auto [current, length] = next_to_visit.top();
        next_to_visit.pop();

        if (visited.contains(current)) continue;
        visited.insert(current);

        if (current == &destination) return length;

        for (const node* connection : current != &start ? current->connections : current->temp_connections) {
            next_to_visit.push({connection, length + 1});
        }
    }

    return -1;
}

size_t find_group_size(const node& start) {
    std::stack<const node*> next_to_visit;
    std::set<const node*> visited;
    size_t group_size = 0;

    next_to_visit.push({&start});

    while (!next_to_visit.empty()) {
        const node* current = next_to_visit.top();
        next_to_visit.pop();

        if (visited.contains(current)) continue;
        visited.insert(current);

        ++group_size;

        for (const node* connection : current->connections) {
            next_to_visit.push(connection);
        }
    }

    return group_size;
}

bool does_two_group_exist(const std::unordered_map<std::string, node> &nodes, const std::set<const node*>& modified_connections) {
    size_t groups_count = 0;
    std::stack<const node*> next_to_visit;
    std::set<const node*> visited;

    for (auto& [name, start] : nodes) {
        if (visited.contains(&start)) continue;

        ++groups_count;

        next_to_visit.push(&start);

        while (!next_to_visit.empty()) {
            const node* current = next_to_visit.top();
            next_to_visit.pop();

            if (visited.contains(current)) continue;
            visited.insert(current);

            for (const node* connection : !modified_connections.contains(current) ? current->connections : current->temp_connections) {
                next_to_visit.push(connection);
            }
        }
    }

    return groups_count == 2;
}

int first_part_2023_25() {
    std::unordered_map<std::string, node> nodes;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE),
                  std::istream_iterator<WordDelimitedBy<'\n'>>(),
                  [&nodes](std::string str) {
                      std::string_view str_view = str;
                      size_t sep_index = str_view.find(':');
                      std::string_view source = str_view.substr(0, sep_index);
                      auto &source_node = nodes[std::string(source)];
                      source_node.name = source;

                      str_view = str_view.substr(sep_index + 2);
                      while (sep_index != std::string::npos) {
                          sep_index = str_view.find(' ');
                          std::string_view dest = str_view.substr(0, sep_index);

                          auto &dest_node = nodes[std::string(dest)];
                          dest_node.name = dest;
                          source_node.connections.push_back(&dest_node);
                          dest_node.connections.push_back(&source_node);

                          str_view = str_view.substr(sep_index + 1);
                      }
                  });

    std::priority_queue<connection_strength, std::vector<connection_strength>, std::less<>> connections;
    std::set<std::pair<std::string, std::string>> tested_connections;

    for (auto&[name, n]: nodes) {
        for (node *connection: n.connections) {
            if (tested_connections.contains({name, connection->name}) ||
                tested_connections.contains({connection->name, name}))
                continue;
            tested_connections.insert({name, connection->name});

            n.temp_connections = n.connections;
            std::erase_if(n.temp_connections, [&connection](node *c) { return c == connection; });

            connection->temp_connections = connection->connections;
            std::erase_if(connection->temp_connections, [&n](node *connection) { return connection == &n; });

            size_t strength = find_shortest_path(n, *connection);

            connections.push({strength, name, connection->name});
        }
    }

    //std::vector<connection_strength> possible_combinations; // See comment below

    std::vector<std::pair<std::string, std::string>> sides;
    std::set<const node *> modified_connections;
    for (size_t i = 0; i < 3; i++) {
        connection_strength c = connections.top();
        connections.pop();

        node* side1_node = &nodes[c.side1];
        node* side2_node = &nodes[c.side2];

        //possible_combinations.push_back(c); // See comment below

        std::cout << c.side1 << " " << c.side2 << " " << c.strength << std::endl;

        sides.emplace_back(c.side1, c.side2);

        modified_connections.insert(side1_node);
        modified_connections.insert(side2_node);

        side1_node->temp_connections = side1_node->connections;
        side2_node->temp_connections = side2_node->connections;

        std::erase_if(side1_node->temp_connections, [c2 = side2_node](const node *c1) {
            return c1 == c2;
        });

        std::erase_if(side2_node->temp_connections, [c1 = side1_node](const node *c2) {
            return c2 == c1;
        });
    }

    if (!does_two_group_exist(nodes, modified_connections)) {
        std::cout << "Error! Can't find two groups.\n";
    }

    for (const node *n: modified_connections) {
        node &non_const_node = nodes[n->name];
        non_const_node.connections = non_const_node.temp_connections;
    }

    auto size1 = find_group_size(nodes[sides[0].first]);
    auto size2 = find_group_size(nodes[sides[0].second]);
    size_t res = size1 * size2;
    std::cout << res << std::endl;

    /*
     // When you are trying to solve an edge case that you encountered, but it was there only because a bug you had:

     while (connections.top().strength == possible_combinations.back().strength) {
        possible_combinations.push_back(connections.top());
        connection_strength c = possible_combinations.back();
        std::cout << c.side1 << " " << c.side2 << " " << c.strength << std::endl;
        connections.pop();
    }

    for (size_t i = 0; i < possible_combinations.size() - 2; ++i) {
        connection_strength possible_1 = possible_combinations[i];
        for (size_t j = i + 1; j < possible_combinations.size() - 1; ++j) {
            connection_strength possible_2 = possible_combinations[j];
            for (size_t k = j + 1; k < possible_combinations.size(); ++k) {
                connection_strength possible_3 = possible_combinations[k];

                std::vector<std::pair<std::string, std::string>> sides;
                std::set<const node *> modified_connections;
                for (const connection_strength &c: {possible_1, possible_2, possible_3}) {
                    sides.emplace_back(c.side1, c.side2);

                    modified_connections.insert(&nodes[c.side1]);
                    modified_connections.insert(&nodes[c.side2]);

                    nodes[c.side1].temp_connections = nodes[c.side1].connections;
                    nodes[c.side2].temp_connections = nodes[c.side2].connections;

                    std::erase_if(nodes[c.side1].temp_connections, [c2 = &nodes[c.side2]](const node *c1) {
                        return c1 == c2;
                    });

                    std::erase_if(nodes[c.side2].temp_connections, [c1 = &nodes[c.side1]](const node *c2) {
                        return c2 == c1;
                    });
                }

                if (does_two_group_exist(nodes, modified_connections)) {
                    for (const node *n: modified_connections) {
                        node &non_const_node = nodes[n->name];
                        non_const_node.connections = non_const_node.temp_connections;
                    }

                    std::cout << "Found relevant connections:\n";
                    for (const auto &s: sides) {
                        std::cout << s.first << " " << s.second << "\n";
                    }

                    auto size1 = find_group_size(nodes[sides[0].first]);
                    auto size2 = find_group_size(nodes[sides[0].second]);
                    size_t res = size1 * size2;
                    std::cout << res << std::endl;

                    return EXIT_SUCCESS;
                }
            }
        }
    }*/

    return EXIT_SUCCESS;
}

int second_part_2023_25() {

    return EXIT_SUCCESS;
}

#endif
