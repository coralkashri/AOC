#ifndef AOC_2020_5
#define AOC_2020_5

#include "../tools/base_includes.h"

void update_place(size_t &current_place, size_t &current_limit, bool is_upper_half) {
    if (is_upper_half) current_place = (current_place + current_limit + 1) / 2;
    else current_limit -= (current_limit + 1 - current_place) / 2;
}

size_t get_location_id(std::string_view str) {
    const size_t rows_count = 128;
    const size_t columns_count = 8;
    size_t current_row = 0, current_row_limit = rows_count - 1;
    size_t current_column = 0, current_column_limit = columns_count - 1;
    for (char c : str) {
        if (c == 'F' || c == 'B') update_place(current_row, current_row_limit, c == 'B');
        else update_place(current_column, current_column_limit, c == 'R');
    }
    return current_row * 8 + current_column;
}

int first_part_2020_5() {
    std::cout << std::transform_reduce(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), 0ul, max<>{}, [&] (std::string str) mutable {
        return get_location_id(str);
    }) << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2020_5() {
    const size_t rows_count = 128;
    const size_t columns_count = 8;
    size_t all_taken_places_sum = 0, all_places_sum = 0;
    size_t last_occupied_location = 0;
    auto first_occupied_location = std::transform_reduce(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), rows_count * 8 + columns_count, min<>{}, [&] (std::string str) mutable {
        size_t current_sit_id = get_location_id(str);
        if (last_occupied_location < current_sit_id) last_occupied_location = current_sit_id;
        all_taken_places_sum += current_sit_id;
        return current_sit_id;
    });
    all_places_sum = (last_occupied_location - first_occupied_location + 1) * (last_occupied_location + first_occupied_location) / 2;

    std::cout << all_places_sum - all_taken_places_sum << std::endl;
    return EXIT_SUCCESS;
}

#endif