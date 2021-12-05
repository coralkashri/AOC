#ifndef AOC_2020_1
#define AOC_2020_1

#include <iostream>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>
#include <thread>
#include <cmath>
#include <iomanip>
#include "../input_source.hpp"

size_t transform_subject_number(size_t value, size_t subject_number) {
    value *= subject_number;
    value %= 20201227;
    return value;
}

size_t full_transform_subject_number(size_t subject_number, long long loop_size) {
    size_t value = 1;
    while (loop_size--) {
        value = transform_subject_number(value, subject_number);
    }
    return value;
}

int first_part_2020_25() {
    const size_t subject_number = 7;
    size_t card_public_key, door_public_key;
    size_t card_loop_size, door_loop_size;
    card_loop_size = door_loop_size = 0;
    INPUT_SOURCE >> card_public_key >> door_public_key;
    std::thread t1([&] {
        auto value = transform_subject_number(1, subject_number);
        card_loop_size++;
        while (value != card_public_key) {
            value = transform_subject_number(value, subject_number);
            card_loop_size++;
        }
    });
    std::thread t2([&] {
        auto value = transform_subject_number(1, subject_number);
        door_loop_size++;
        while (value != door_public_key) {
            value = transform_subject_number(value, subject_number);
            door_loop_size++;
        }
    });
    t1.join();
    t2.join();
    std::cout << "Encryption key: " << full_transform_subject_number(card_public_key, door_loop_size) << std::endl;
    std::cout << "Encryption key: " << full_transform_subject_number(door_public_key, card_loop_size) << std::endl;
    return EXIT_SUCCESS;
}

#endif