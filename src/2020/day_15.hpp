#ifndef AOC_2020_15
#define AOC_2020_15

#include "../tools/base_includes.h"

class memory {
private:
    using inner_mem_t = std::vector<std::pair<uint32_t, uint32_t>>;

public:
    memory() : current_turn(1), inner_mem(20) {
        read_initial_numbers();
    }

    inner_mem_t::size_type get_spoken_number_on_turn(inner_mem_t::value_type::first_type turn) {
        inner_mem.resize(turn);
        while (current_turn <= turn) call_next_number();
        return last_spoken_number;
    }

private:
    inner_mem_t inner_mem;
    inner_mem_t::size_type last_spoken_number;
    inner_mem_t::value_type::first_type current_turn;

    void read_initial_numbers() {
        std::for_each(std::istream_iterator<WordDelimitedBy<','>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<','>>(), [this] (std::string str) mutable {
            last_spoken_number = std::stoi(str);
            inner_mem[last_spoken_number] = {current_turn, 0};
            ++current_turn;
        });
    }

    void call_next_number() {
        auto &last_spoken_number_data = inner_mem.at(last_spoken_number);
        if (!last_spoken_number_data.second) last_spoken_number = 0;
        else last_spoken_number = last_spoken_number_data.first - last_spoken_number_data.second;
        if (inner_mem[last_spoken_number].first) {
            inner_mem[last_spoken_number] = {current_turn, inner_mem[last_spoken_number].first};
        } else {
            inner_mem[last_spoken_number] = {current_turn, 0};
        }
        ++current_turn;
    }
};

int first_part_2020_15() {
    memory m;

    std::cout << m.get_spoken_number_on_turn(2020) << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2020_15() {
    memory m;

    std::cout << m.get_spoken_number_on_turn(30000000) << std::endl;

    return EXIT_SUCCESS;
}

#endif