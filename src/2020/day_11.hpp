#ifndef AOC_2020_11
#define AOC_2020_11

#include "../tools/base_includes.h"

using matrix_t = std::unordered_map<point_xd<2>, bool>;

template <bool ImmediateView>
size_t compute_occupied_neighbors_count(const matrix_t &mat, size_t lines_count, size_t rows_count, const matrix_t::key_type &place) {
    size_t counter = 0;
    static const std::vector<std::pair<int, int>> offsets = {
            {-1, -1}, {-1, 0}, {-1, 1},
            {0, -1},                 {0, 1},
            {1, -1},  {1, 0},  {1, 1}
    };
    for (auto const &offset : offsets) {
        auto current_place = matrix_t::key_type{place[0] + offset.first, place[1] + offset.second};
        if constexpr (!ImmediateView) {
            while (mat.find(current_place) == mat.end() && current_place[0] < lines_count && current_place[1] < rows_count && current_place[0] >= 0 && current_place[1] >= 0) {
                current_place[0] += offset.first;
                current_place[1] += offset.second;
            }
            if (current_place[0] < lines_count && current_place[1] < rows_count && current_place[0] >= 0 && current_place[1] >= 0) counter += mat.at(current_place);
        } else if (mat.find(current_place) != mat.end()) counter += mat.at(current_place);
    }
    return counter;
}

template <size_t Tolerance, bool ImmediateView>
bool update_matrix_cycle(matrix_t &mat, matrix_t &new_mat, size_t lines_count, size_t rows_count) {
    bool is_update_occurred = false;
    for (const auto &place : mat) {
        auto occupied_neighbors_count = compute_occupied_neighbors_count<ImmediateView>(mat, lines_count, rows_count, place.first);
        if ((place.second && occupied_neighbors_count >= Tolerance) || (!place.second && !occupied_neighbors_count)) {
            new_mat.at(place.first) = !place.second;
            is_update_occurred = true;
        }
        else new_mat.at(place.first) = place.second;
    }
    std::swap(mat, new_mat);
    return is_update_occurred;
}

void print_matrix(const matrix_t &mat) {
    std::map<matrix_t::key_type, bool> ordered_mat;
    auto max_line_size = 0l;
    for (const auto &place : mat) {
        ordered_mat[place.first] = place.second;
        max_line_size = std::max(max_line_size, place.first[1]);
    }
    size_t last_line = 0;
    size_t next_x = 0;
    for (const auto &place : ordered_mat) {
        if (place.first[0] != last_line) {
            while (next_x++ <= max_line_size) std::cout << ".";
            std::cout << "\n";
            next_x = 0;
        }
        while (place.first[1] > next_x) {
            std::cout << ".";
            next_x++;
        }
        std::cout << (place.second ? '#' : 'L');
        next_x++;
        last_line = place.first[0];
    }
    std::cout << "\n\n";
}

auto input_matrix(matrix_t &matrix) {
    auto lines_count = 0ll, rows_count = 0ll;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (std::string str) mutable {
        rows_count = str.size();
        for (int64_t i = 0; i < str.size(); i++) {
            if (str[i] != '.') matrix[{lines_count, i}] = str[i] == '#';
        }
        lines_count++;
    });
    return std::pair{lines_count, rows_count};
}

size_t count_occupied_points_in_matrix(const matrix_t &matrix) {
    return std::count_if(matrix.begin(), matrix.end(), [] (const auto &place) {
        return place.second;
    });
}

int first_part_2020_11() {
    matrix_t matrix;
    auto [lines_count, rows_count] = input_matrix(matrix);

    auto new_mat = matrix;
    while (update_matrix_cycle<4, true>(matrix, new_mat, lines_count, rows_count));

    std::cout << count_occupied_points_in_matrix(matrix) << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2020_11() {
    matrix_t matrix;
    auto [lines_count, rows_count] = input_matrix(matrix);

    auto new_mat = matrix;
    while (update_matrix_cycle<5, false>(matrix, new_mat, lines_count, rows_count));

    std::cout << count_occupied_points_in_matrix(matrix) << std::endl;

    return EXIT_SUCCESS;
}

#endif