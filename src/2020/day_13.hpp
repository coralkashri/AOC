#ifndef AOC_2020_13
#define AOC_2020_13

#include "../tools/base_includes.h"

int first_part_2020_13() {
    uint min_waiting_time = -1;
    uint best_bus_id;
    uint self_arriving_time;
    INPUT_SOURCE >> self_arriving_time;
    std::for_each(std::istream_iterator<WordDelimitedBy<','>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<','>>(), [&] (std::string str) mutable {
        if (str == "x") return;
        uint current_bus_id = std::stoi(str);
        uint current_waiting_time = current_bus_id - self_arriving_time % current_bus_id;
        if (min_waiting_time > current_waiting_time) {
            min_waiting_time = current_waiting_time;
            best_bus_id = current_bus_id;
        }
    });

    std::cout << best_bus_id * min_waiting_time << std::endl;

    return EXIT_SUCCESS;
}

uint64_t perform_iteration(std::vector<std::pair<uint, uint>> &all_bus_ids, std::unordered_map<uint, unsigned long long> &arrivals) {
    uint64_t res = 1;
    uint64_t mode = 1;
    for (auto &bus_id : all_bus_ids) {
        /* // lazy solution:
        if (bus_id.second == 0) arrivals[bus_id.first] += bus_id.first;
        else {
            if (arrivals[all_bus_ids[0].first] >= arrivals[bus_id.first])
                arrivals[bus_id.first] += bus_id.first * (unsigned long long)std::ceil(((double)arrivals[all_bus_ids[0].first] - (double)arrivals[bus_id.first]) / (double)bus_id.first);
        }*/
        while ((res + bus_id.second) % bus_id.first) {
            res += mode;
        }
        mode *= bus_id.first;
    }
    /* // lazy solution:
    for (auto &bus_id : all_bus_ids) {
        res = res && arrivals[bus_id.first] - arrivals[all_bus_ids[0].first] == bus_id.second;
    }*/
    return res;
}

int second_part_2020_13() {
    std::vector<std::pair<uint, uint>> all_bus_ids;
    std::unordered_map<uint, unsigned long long> arrivals;
    size_t self_arriving_time;
    INPUT_SOURCE >> self_arriving_time;
    std::for_each(std::istream_iterator<WordDelimitedBy<','>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<','>>(), [&, current_time = 0] (std::string str) mutable {
        if (str == "x") {
            current_time++;
            return;
        }
        uint current_bus_id = std::stoi(str);
        all_bus_ids.emplace_back(current_bus_id, current_time);
        current_time++;
    });

    for (auto &bus_id : all_bus_ids) {
        arrivals[bus_id.first] = 0;
    }

    std::cout << perform_iteration(all_bus_ids, arrivals) << std::endl;
// lazy solution: //    std::cout << arrivals[all_bus_ids[0].first] << std::endl;

    return EXIT_SUCCESS;
}

#endif