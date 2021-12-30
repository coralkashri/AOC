#ifndef AOC_2020_17
#define AOC_2020_17

#include "../tools/base_includes.h"
#include <stack>
#include <unordered_set>
#include <atomic>
#include <thread>

struct point {
    int64_t x, y, z;

    bool operator<(const point &ref) const {
        return std::tie(x, y, z) < std::tie(ref.x, ref.y, ref.z);
    }

    bool operator==(const point& ref) const {
        return std::tie(x, y, z) == std::tie(ref.x, ref.y, ref.z);
    }
};

using matrix_t = std::map<point, bool>;

size_t get_neighbors_sum(const matrix_t &matrix, matrix_t &new_matrix, point p) {
    size_t sum = 0;
    for (auto x = p.x - 1; x <= p.x + 1; x++) {
        for (auto y = p.y - 1; y <= p.y + 1; y++) {
            for (auto z = p.z - 1; z <= p.z + 1; z++) {
                sum += matrix.find(point{x, y, z}) != matrix.end() && matrix.at(point{x, y, z});
                new_matrix[point{x, y, z}];
            }
        }
    }
    sum -= matrix.at(p);
    return sum;
}

void prepare_all_neighbors(matrix_t &matrix) {
    matrix_t matrix_with_all_neighbors = matrix;
    for (auto &data : matrix) {
        get_neighbors_sum(matrix, matrix_with_all_neighbors, data.first);
    }
    matrix = matrix_with_all_neighbors;
}

void perform_cycle(matrix_t &matrix) {
    prepare_all_neighbors(matrix);
    matrix_t matrix_after_cycle;
    for (auto &data : matrix) {
        if (data.first.z < 0) continue;
        auto active_neighbors_count = get_neighbors_sum(matrix, matrix_after_cycle, data.first);
        matrix_after_cycle[data.first] = false;
        if (data.second) { // The point currently active
            if (active_neighbors_count == 2 || active_neighbors_count == 3) matrix_after_cycle[data.first] = true;
        } else { // The point currently inactive
            if (active_neighbors_count == 3) matrix_after_cycle[data.first] = true;
        }
    }
    matrix = matrix_after_cycle;
}

int first_part_2020_17() {
    matrix_t matrix;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&, current_line = 0] (std::string str) mutable {
        size_t x_pos = 0;
        str = "." + str + ".";
        for (char c : str) {
            matrix[{(int64_t)x_pos, current_line, 0}] = c == '#';
            x_pos++;
        }
        current_line++;
    });

    for (size_t i = 0; i < 6; i++)
        perform_cycle(matrix);

    std::cout << std::count_if(matrix.begin(), matrix.end(), [](const matrix_t::value_type &v) {
        return v.second;
    }) << std::endl;
    return EXIT_SUCCESS;
}

struct point_4d {
    int64_t x, y, z, w;

    bool operator<(const point_4d &ref) const {
        return std::tie(x, y, z, w) < std::tie(ref.x, ref.y, ref.z, ref.w);
    }

    bool operator==(const point_4d& ref) const {
        return std::tie(x, y, z, w) == std::tie(ref.x, ref.y, ref.z, ref.w);
    }
};

using matrix_4d_t = std::map<point_4d, bool>;

size_t get_neighbors_sum_4d(const matrix_4d_t &matrix, matrix_4d_t &new_matrix, point_4d p) {
    size_t sum = 0;
    for (auto x = p.x - 1; x <= p.x + 1; x++) {
        for (auto y = p.y - 1; y <= p.y + 1; y++) {
            for (auto z = p.z - 1; z <= p.z + 1; z++) {
                for (auto w = p.w - 1; w <= p.w + 1; w++) {
                    sum += matrix.find(point_4d{x, y, z, w}) != matrix.end() && matrix.at(point_4d{x, y, z, w});
                    new_matrix[{x, y, z, w}];
                }
            }
        }
    }
    sum -= matrix.at(p);
    return sum;
}

void prepare_all_neighbors_4d(matrix_4d_t &matrix) {
    matrix_4d_t matrix_with_all_neighbors = matrix;
    for (auto &data : matrix) {
        get_neighbors_sum_4d(matrix, matrix_with_all_neighbors, data.first);
    }
    matrix = matrix_with_all_neighbors;
}

void perform_cycle_4d(matrix_4d_t &matrix) {
    prepare_all_neighbors_4d(matrix);
    matrix_4d_t matrix_after_cycle;
    for (auto &data : matrix) {
        auto active_neighbors_count = get_neighbors_sum_4d(matrix, matrix_after_cycle, data.first);
        matrix_after_cycle[data.first] = false;
        if (data.second) { // The point currently active
            if (active_neighbors_count == 2 || active_neighbors_count == 3) matrix_after_cycle[data.first] = true;
        } else { // The point currently inactive
            if (active_neighbors_count == 3) matrix_after_cycle[data.first] = true;
        }
    }
    matrix = matrix_after_cycle;
}

int second_part_2020_17() {
    matrix_4d_t matrix;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&, current_line = 0] (std::string str) mutable {
        size_t x_pos = 0;
        for (char c : str) {
            matrix[{(int64_t)x_pos, current_line, 0, 0}] = c == '#';
            x_pos++;
        }
        current_line++;
    });

    for (size_t i = 0; i < 6; i++)
        perform_cycle_4d(matrix);

    std::cout << std::count_if(matrix.begin(), matrix.end(), [](const matrix_4d_t ::value_type &v) {
        return v.second;
    }) << std::endl;
    return EXIT_SUCCESS;
}

#endif