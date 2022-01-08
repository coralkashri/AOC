#ifndef AOC_2020_11
#define AOC_2020_11

#include "../tools/base_includes.h"

using matrix_t = std::vector<std::vector<int>>;

template <bool ImmediateView>
size_t compute_occupied_neighbors_count(const matrix_t &mat, const point_xd<2> &place) {
    size_t counter = 0;
    static const std::vector<std::pair<int, int>> offsets = {
            {-1, -1}, {-1, 0}, {-1, 1},
            {0, -1},                 {0, 1},
            {1, -1},  {1, 0},  {1, 1}
    };
    static const auto test_bounds = [&mat] (const auto &place) {
        return place[0] < mat.size() - 1 && place[1] < mat[0].size() - 1 && place[0] > 0 && place[1] > 0;
    };
    for (auto const &offset : offsets) {
        auto current_place = point_xd<2>{place[0] + offset.first, place[1] + offset.second};
        if constexpr (!ImmediateView) {
            while (test_bounds(current_place) && mat[current_place[0]][current_place[1]] == -1) {
                current_place[0] += offset.first;
                current_place[1] += offset.second;
            }
            if (test_bounds(current_place)) counter += mat[current_place[0]][current_place[1]];
        } else if (mat[current_place[0]][current_place[1]] != -1) counter += mat[current_place[0]][current_place[1]];
    }
    return counter;
}

template <size_t Tolerance, bool ImmediateView>
bool update_matrix_cycle(matrix_t &mat, matrix_t &new_mat) {
    bool is_update_occurred = false;
    for (auto i = 1l; i < mat.size() - 1; i++) {
        for (auto j = 1l; j < mat[0].size() - 1; j++) {
            if (mat[i][j] == -1) continue;
            auto occupied_neighbors_count = compute_occupied_neighbors_count<ImmediateView>(mat, {i, j});
            if ((mat[i][j] == 1 && occupied_neighbors_count >= Tolerance) || (mat[i][j] == 0 && !occupied_neighbors_count)) {
                new_mat[i][j] = !mat[i][j];
                is_update_occurred = true;
            } else new_mat[i][j] = mat[i][j];
        }
    }
    std::swap(mat, new_mat);
    return is_update_occurred;
}

void input_matrix(matrix_t &matrix) {
    matrix.reserve(100);
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (std::string str) mutable {
        auto &current_line = matrix.emplace_back();
        str = "." + str + ".";
        current_line.reserve(str.size());
        for (const auto &c : str) {
            current_line.emplace_back(c == '.' ? -1 : c == '#' ? 1 : 0);
        }
    });
    matrix.insert(matrix.begin(), std::vector<int>(matrix[0].size(), -1));
    matrix.insert(matrix.end(), std::vector<int>(matrix[0].size(), -1));
}

size_t count_occupied_points_in_matrix(const matrix_t &matrix) {
    return std::transform_reduce(matrix.begin(), matrix.end(), 0ul, std::plus<>{}, [] (const auto &line) {
        return std::count_if(line.begin(), line.end(), [] (auto &val) {
            return val == 1;
        });
    });
}

int first_part_2020_11() {
    matrix_t matrix;
    input_matrix(matrix);

    auto new_mat = matrix;
    while (update_matrix_cycle<4, true>(matrix, new_mat));

    std::cout << count_occupied_points_in_matrix(matrix) << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2020_11() {
    matrix_t matrix;
    input_matrix(matrix);

    auto new_mat = matrix;
    while (update_matrix_cycle<5, false>(matrix, new_mat));

    std::cout << count_occupied_points_in_matrix(matrix) << std::endl;

    return EXIT_SUCCESS;
}

#endif