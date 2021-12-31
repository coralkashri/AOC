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

using matrix_t = std::vector<std::vector<std::vector<int>>>;

size_t get_neighbors_sum(const matrix_t &matrix, point p) {
    size_t sum = 0;
    for (auto x = p.x - 1; x <= p.x + 1; x++) {
        for (auto y = p.y - 1; y <= p.y + 1; y++) {
            for (auto z = p.z - 1; z <= p.z + 1; z++) {
                sum += matrix[x][y][std::abs(z)];
            }
        }
    }
    sum -= matrix[p.x][p.y][p.z];
    return sum;
}

void prepare_matrix(matrix_t &matrix, size_t y_dim_size) {
    matrix.insert(matrix.begin(), 2, std::vector<std::vector<int>>(y_dim_size, std::vector<int>(matrix[0][0].size(), 0)));
    matrix.insert(matrix.end(), 2, std::vector<std::vector<int>>(y_dim_size, std::vector<int>(matrix[0][0].size(), 0)));
    auto line_size = matrix[0][0].size();
    for (auto &line : matrix) {
        line.insert(line.begin(), 2, std::vector<int>(line_size, 0));
        line.insert(line.end(), 2, std::vector<int>(line_size, 0));
        for (auto &dims : line) {
            dims.insert(dims.end(), 2, 0);
        }
    }
}

void reset_matrix(matrix_t &matrix) {
    for (auto &line : matrix) {
        for (auto &dims : line) {
            std::fill(dims.begin(), dims.end(), 0);
        }
    }
}

void perform_cycle(matrix_t &matrix) {
    prepare_matrix(matrix, matrix[0].size());
    matrix_t matrix_after_cycle = matrix;
    reset_matrix(matrix_after_cycle);
    for (int64_t x_pos = 1; x_pos < matrix.size() - 1; x_pos++) {
        for (int64_t y_pos = 1; y_pos < matrix[0].size() - 1; y_pos++) {
            for (int64_t dim_pos = 0; dim_pos < matrix[0][0].size() - 1; dim_pos++) {
                auto active_neighbors_count = get_neighbors_sum(matrix, point{x_pos, y_pos, dim_pos});
                matrix_after_cycle[x_pos][y_pos][dim_pos] = 0;
                if (matrix[x_pos][y_pos][dim_pos]) { // The point currently active
                    if (active_neighbors_count == 2 || active_neighbors_count == 3)
                        matrix_after_cycle[x_pos][y_pos][dim_pos] = true;
                } else { // The point currently inactive
                    if (active_neighbors_count == 3) matrix_after_cycle[x_pos][y_pos][dim_pos] = true;
                }
            }
        }
    }
    matrix = matrix_after_cycle;
}

int first_part_2020_17() {
    matrix_t matrix;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (std::string str) mutable {
        auto &current_x = matrix.emplace_back();
        for (char c : str) {
            current_x.emplace_back().emplace_back() = c == '#';
        }
    });

    for (size_t i = 0; i < 6; i++)
        perform_cycle(matrix);

    std::cout << std::transform_reduce(matrix.begin(), matrix.end(), 0, std::plus<>{}, [](const matrix_t::value_type &v) {
        return std::transform_reduce(v.begin(), v.end(), 0, std::plus<>{}, [] (const std::vector<int> &v1) {
            return std::reduce(v1.begin(), v1.end());
        });
    }) * 2 - std::transform_reduce(matrix.begin(), matrix.end(), 0, std::plus<>{}, [](const matrix_t::value_type &v) {
        return std::transform_reduce(v.begin(), v.end(), 0, std::plus<>{}, [] (const std::vector<int> &v1) {
            return v1[0];
        });
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

using matrix_4d_t = std::vector<matrix_t>;

size_t get_neighbors_sum(const matrix_4d_t &matrix, point_4d p) {
    size_t sum = 0;
    for (auto x = p.x - 1; x <= p.x + 1; x++) {
        for (auto y = p.y - 1; y <= p.y + 1; y++) {
            for (auto z = p.z - 1; z <= p.z + 1; z++) {
                for (auto w = p.w - 1; w <= p.w + 1; w++) {
                    sum += matrix[x][y][std::abs(z)][std::abs(w)];
                }
            }
        }
    }
    sum -= matrix[p.x][p.y][p.z][p.w];
    return sum;
}

void prepare_matrix(matrix_4d_t &matrix) {
    static bool is_first_time = true;
    matrix.insert(matrix.begin(), 1 + is_first_time, matrix_t(matrix[0].size(), std::vector<std::vector<int>>(matrix[0][0].size(), std::vector<int>(matrix[0][0][0].size(), 0))));
    matrix.insert(matrix.end(), 1 + is_first_time, matrix_t(matrix[0].size(), std::vector<std::vector<int>>(matrix[0][0].size(), std::vector<int>(matrix[0][0][0].size(), 0))));
    auto line_size = matrix[0][0].size();
    auto dim0_size = matrix[0][0][0].size();
    for (auto &line : matrix) {
        line.insert(line.begin(), 1 + is_first_time, std::vector<std::vector<int>>(line_size, std::vector<int>(dim0_size, 0)));
        line.insert(line.end(), 1 + is_first_time, std::vector<std::vector<int>>(line_size, std::vector<int>(dim0_size, 0)));
        for (auto &dim0s : line) {
            dim0s.insert(dim0s.end(), 1 + is_first_time, std::vector<int>(dim0_size, 0));
            for (auto &dim1s : dim0s) {
                dim1s.insert(dim1s.end(), 1 + is_first_time, 0);
            }
        }
    }
    is_first_time = false;
}

void reset_matrix(matrix_4d_t &matrix) {
    for (auto &line : matrix) {
        reset_matrix(line);
    }
}

void perform_cycle(matrix_4d_t &matrix) {
    prepare_matrix(matrix);
    auto matrix_after_cycle = matrix;
    reset_matrix(matrix_after_cycle);
    for (int64_t x_pos = 1; x_pos < matrix.size() - 1; x_pos++) {
        for (int64_t y_pos = 1; y_pos < matrix[0].size() - 1; y_pos++) {
            for (int64_t dim_pos = 0; dim_pos < matrix[0][0].size() - 1; dim_pos++) {
                for (int64_t dim1_pos = 0; dim1_pos < matrix[0][0][0].size() - 1; dim1_pos++) {
                    auto active_neighbors_count = get_neighbors_sum(matrix, point_4d{x_pos, y_pos, dim_pos, dim1_pos});
                    matrix_after_cycle[x_pos][y_pos][dim_pos][dim1_pos] = 0;
                    if (matrix[x_pos][y_pos][dim_pos][dim1_pos]) { // The point currently active
                        if (active_neighbors_count == 2 || active_neighbors_count == 3)
                            matrix_after_cycle[x_pos][y_pos][dim_pos][dim1_pos] = true;
                    } else { // The point currently inactive
                        if (active_neighbors_count == 3) matrix_after_cycle[x_pos][y_pos][dim_pos][dim1_pos] = true;
                    }
                }
            }
        }
    }
    matrix = matrix_after_cycle;
}

void print_matrix(const matrix_4d_t &matrix) {
    size_t current_dim_1 = 0;
    while (current_dim_1 < matrix[0][0][0].size() - 1) {
        size_t current_dim = 0;
        while (current_dim < matrix[0][0].size() - 1) {
            std::cout << "Z=" << current_dim << " W=" << current_dim_1 << std::endl;
            std::for_each(matrix.begin(), matrix.end(), [current_dim, current_dim_1](const matrix_4d_t::value_type &v) {
                std::for_each(v.begin(), v.end(), [current_dim, current_dim_1](const matrix_t::value_type &v1) {
                    std::cout << (v1[current_dim][current_dim_1] ? std::to_string(v1[current_dim][current_dim_1]) : ".") << " ";
                });
                std::cout << std::endl;
            });
            current_dim++;
            std::cout << "\n\n";
        }
        current_dim_1++;
        std::cout << "\n\n";
    }
}

size_t compute_matrix_sum(matrix_4d_t &matrix) {
    std::for_each(matrix.begin(), matrix.end(), [](matrix_4d_t::value_type &v) {
        std::for_each(v.begin(), v.end(), [](matrix_t::value_type &v) {
            std::for_each(v.begin(), v.end(), [curr_z_dim = 0] (std::vector<int> &v1) mutable {
                int curr_w_dim = 0;
                std::for_each(v1.begin(), v1.end(), [&curr_z_dim, &curr_w_dim] (int &v2) mutable {
                    if (curr_z_dim != -curr_z_dim && curr_w_dim != -curr_w_dim) v2 *= 2;
                    curr_w_dim++;
                });
                curr_z_dim++;
            });
        });
    });

    return std::transform_reduce(matrix.begin(), matrix.end(), 0, std::plus<>{}, [](const matrix_4d_t::value_type &v) {
        return std::transform_reduce(v.begin(), v.end(), 0, std::plus<>{}, [](const matrix_t::value_type &v) {
            return std::transform_reduce(v.begin(), v.end(), 0, std::plus<>{}, [] (const std::vector<int> &v1) {
                return std::reduce(v1.begin(), v1.end());
            });
        });
    }) * 2 - std::transform_reduce(matrix.begin(), matrix.end(), 0, std::plus<>{}, [](const matrix_4d_t::value_type &v) {
        return std::transform_reduce(v.begin(), v.end(), 0, std::plus<>{}, [](const matrix_t::value_type &v) {
            return v[0][0];
        });
    });
}

int second_part_2020_17() {
    matrix_4d_t matrix;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (std::string str) mutable {
        auto &current_x = matrix.emplace_back();
        for (char c : str) {
            current_x.emplace_back().emplace_back().emplace_back() = c == '#';
        }
    });

    for (size_t i = 0; i < 6; i++) {
        perform_cycle(matrix);
    }

    std::cout << compute_matrix_sum(matrix) << std::endl;
    return EXIT_SUCCESS;
}

#endif