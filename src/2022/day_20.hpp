#ifndef AOC_2022_20
#define AOC_2022_20

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include <variant>
#include <array>
#include <numeric>
#include <set>

#include "../input_source.hpp"

auto mix(std::list<int64_t> &numbers, std::vector<std::remove_reference_t<decltype(numbers)>::iterator> &ordered_nodes) {
    std::remove_reference_t<decltype(numbers)>::iterator zero_node;

    std::vector<std::remove_reference_t<decltype(numbers)>::iterator> new_ordered_nodes;
    new_ordered_nodes.reserve(ordered_nodes.size());

    for (auto n: ordered_nodes) {
        auto number = *n;
        int64_t index = std::distance(numbers.begin(), n);
        int64_t new_index = 0;

        if (number != 0) {
            numbers.erase(n);
        }

        new_index = (index + number) % ssize(numbers);

        if (new_index < 0) {
            new_index += ssize(numbers);
        }

        if (number != 0) {
            new_ordered_nodes.push_back(numbers.insert(std::next(numbers.begin(), new_index), number));
        } else {
            new_ordered_nodes.push_back(n);
            zero_node = n;
        }
    }

    ordered_nodes = new_ordered_nodes;

    return zero_node;
}

void resolve_decryption(std::list<int64_t> &numbers, std::remove_reference_t<decltype(numbers)>::iterator zero_node_it) {
    int64_t zero_index = std::distance(numbers.begin(), zero_node_it);
    int64_t first_index = (zero_index + 1000) % ssize(numbers);
    int64_t second_index = (zero_index + 2000) % ssize(numbers);
    int64_t third_index = (zero_index + 3000) % ssize(numbers);

    auto first_node = std::next(numbers.begin(), first_index);
    auto second_node = std::next(numbers.begin(), second_index);
    auto third_node = std::next(numbers.begin(), third_index);

    std::cout << *first_node << " " << *second_node << " " << *third_node << std::endl;

    std::cout << (*first_node + *second_node + *third_node) << std::endl;
}

std::list<int64_t> read_numbers(int64_t decryption_key = 1) {
    std::list<int64_t> numbers;
    std::transform(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(),
                   std::back_inserter(numbers), [decryption_key](const std::string &str) {
                return std::stoll(str) * decryption_key;
            });
    return numbers;
}

auto initialize_nodes_vector(std::list<int64_t> &numbers) {
    std::vector<std::remove_reference_t<decltype(numbers)>::iterator> nodes;

    nodes.reserve(numbers.size());
    for (auto it = numbers.begin(); it != numbers.end(); ++it) {
        nodes.push_back(it);
    }

    return nodes;
}

int first_part_2022_20() {
    const std::list<int64_t> original_numbers = read_numbers();

    std::list<int64_t> current_numbers = original_numbers;
    std::vector<decltype(current_numbers)::iterator> nodes = initialize_nodes_vector(current_numbers);

    auto zero_node_it = mix(current_numbers, nodes);

    resolve_decryption(current_numbers, zero_node_it);

    return EXIT_SUCCESS;
}

int second_part_2022_20() {
    const std::list<int64_t> original_numbers = read_numbers(811589153ll);

    std::list<int64_t> current_numbers = original_numbers;
    std::vector<decltype(current_numbers)::iterator> nodes = initialize_nodes_vector(current_numbers);

    decltype(current_numbers)::iterator zero_node_it;

    for (size_t i = 0; i < 10; ++i) {
        zero_node_it = mix(current_numbers, nodes);
    }

    resolve_decryption(current_numbers, zero_node_it);

    return EXIT_SUCCESS;
}

#endif