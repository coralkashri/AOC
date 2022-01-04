#ifndef AOC_2020_5
#define AOC_2020_5

#include "../tools/base_includes.h"
#include <stack>
#include <unordered_set>
#include <atomic>
#include <thread>

void update_place(size_t &current_place, size_t &current_limit, bool is_upper_half) {
    if (is_upper_half) current_place = (current_place + current_limit + 1) / 2;
    else current_limit -= (current_limit + 1 - current_place) / 2;
}

int first_part_2020_5() {
    const size_t rows_count = 128;
    const size_t columns_count = 8;
    std::cout << std::transform_reduce(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), 0ul, max<>{}, [&] (std::string str) mutable {
        size_t current_row = 0, current_row_limit = rows_count - 1;
        size_t current_column = 0, current_column_limit = columns_count - 1;
        for (char c : str) {
            if (c == 'F' || c == 'B') update_place(current_row, current_row_limit, c == 'B');
            else update_place(current_column, current_column_limit, c == 'R');
        }
        return current_row * 8 + current_column;
    }) << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2020_5() {
    const size_t rows_count = 128;
    const size_t columns_count = 8;
    std::vector<unsigned char> all_sits(rows_count * 8 + columns_count);
    auto first_occupied_location = std::transform_reduce(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), all_sits.size(), min<>{}, [&] (std::string str) mutable {
        size_t current_row = 0, current_row_limit = rows_count - 1;
        size_t current_column = 0, current_column_limit = columns_count - 1;
        for (char c : str) {
            if (c == 'F' || c == 'B') update_place(current_row, current_row_limit, c == 'B');
            else update_place(current_column, current_column_limit, c == 'R');
        }
        size_t current_sit_id = current_row * 8 + current_column;
        all_sits.at(current_sit_id - 1) = 1;
        return current_sit_id;
    });

    auto my_location = std::find(std::next(all_sits.begin(), (int)first_occupied_location), all_sits.end(), 0);
    std::cout << std::distance(all_sits.begin(), my_location) + 1 << std::endl;
    return EXIT_SUCCESS;
}

#endif