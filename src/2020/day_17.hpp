#ifndef AOC_2020_17
#define AOC_2020_17

#include "../tools/base_includes.h"
#include <stack>
#include <unordered_set>
#include <atomic>
#include <thread>

template <size_t Dims>
struct point_xd {
    std::array<int64_t, Dims> places;

    bool operator<(const point_xd<Dims> &ref) const {
        return places < ref.places;
    }

    bool operator==(const point_xd& ref) const {
        return places == ref.places;
    }

    auto& operator[](size_t dim) {
        return places[dim];
    }
};

template <size_t Dims>
struct matrix_xd_s {
    using type = std::vector<typename matrix_xd_s<Dims - 1>::type>;

    template <size_t CurrentDim = 0>
    static int get_elem_in_point(const auto &matrix, point_xd<Dims> p) {
        if constexpr (CurrentDim == Dims) return matrix;
        else return get_elem_in_point<CurrentDim + 1>(matrix[std::abs(p[CurrentDim])], p);
    }

    template <size_t CurrentDim = 0>
    static void get_all_dim_sizes(const auto &matrix, point_xd<Dims> &sizes) {
        if constexpr (CurrentDim == Dims) {
            sizes[CurrentDim] = 1;
            return;
        } else {
            sizes[CurrentDim] = matrix.size();
            get_all_dim_sizes<CurrentDim + 1>(matrix[0], sizes);
        }
    }

    template <size_t CurrentDim = 0>
    static size_t get_matrix_sum(const auto &matrix) {
        if constexpr (CurrentDim <= Dims - 2) {
            return std::transform_reduce(matrix.begin(), matrix.end(), 0, std::plus<>{}, [] (const auto &inner_dim) {
                return get_matrix_sum<CurrentDim + 1>(inner_dim);
            });
        } else {
            return std::reduce(matrix.begin(), matrix.end());
        }
    }

    template <size_t CurrentDim = 0>
    static size_t get_matrix_zero_dims_sum(const auto &matrix) {
        if constexpr (CurrentDim <= 1) {
            return std::transform_reduce(matrix.begin(), matrix.end(), 0, std::plus<>{}, [] (const auto &inner_dim) {
                return get_matrix_zero_dims_sum<CurrentDim + 1>(inner_dim);
            });
        } else {
            if constexpr (CurrentDim <= Dims - 2) return get_matrix_zero_dims_sum<CurrentDim + 1>(matrix[0]);
            else return matrix[0];
        }
    }

    template <size_t CurrentDim = 0>
    static void compute_negative_dimensions(auto &matrix, bool is_prev_dim_number_has_neg_dim = true) {
        if constexpr (Dims <= 3) return;
        if constexpr (CurrentDim <= 1) {
            std::for_each(matrix.begin(), matrix.end(), [] (auto &inner_dim) {
                compute_negative_dimensions<CurrentDim + 1>(inner_dim, true);
            });
        } else {
            std::for_each(matrix.begin(), matrix.end(), [is_prev_dim_number_has_neg_dim, current_dim_number = 0] (auto &inner_dim) mutable {
                if constexpr (CurrentDim <= Dims - 2) {
                    compute_negative_dimensions<CurrentDim + 1>(inner_dim, is_prev_dim_number_has_neg_dim && current_dim_number != -current_dim_number);
                } else {
                    if (is_prev_dim_number_has_neg_dim && current_dim_number != -current_dim_number) {
                        inner_dim *= 2;
                    }
                }
                current_dim_number++;
            });
        }
    }
};

template <>
struct matrix_xd_s<1> {
    using type = std::vector<int>;
};

template <size_t Dims>
using matrix_xd_s_t = typename matrix_xd_s<Dims>::type;

template <size_t Dims, size_t CurrentDim = 0>
size_t get_neighbors_sum_helper(const auto &matrix, point_xd<Dims> p) {
    if constexpr (CurrentDim == Dims) return matrix;
    else {
        size_t sum = 0;
        for (auto place = p[CurrentDim] - 1; place <= p[CurrentDim] + 1; place++) {
            sum += get_neighbors_sum_helper<Dims, CurrentDim + 1>(matrix[std::abs(place)], p);
        }
        return sum;
    }
}

template <size_t Dims>
size_t get_neighbors_sum(const matrix_xd_s_t<Dims> &matrix, point_xd<Dims> p) {
    size_t sum = get_neighbors_sum_helper(matrix, p);
    sum -= matrix_xd_s<Dims>::get_elem_in_point(matrix, p);
    return sum;
}

template <size_t Dims, size_t CurrentDim = 1>
void prepare_matrix_helper(auto &matrix, point_xd<Dims> target_sizes) {
    for (auto &dim : matrix) {
        if constexpr (CurrentDim < 2) {
            dim.resize(target_sizes[CurrentDim] + 2);
            std::shift_right(dim.begin(), dim.end(), 1);
        } else dim.resize(target_sizes[CurrentDim] + 1);
        if constexpr (CurrentDim < Dims - 1) prepare_matrix_helper<Dims, CurrentDim + 1>(dim, target_sizes);
    }
}

template <size_t Dims>
void prepare_matrix(matrix_xd_s_t<Dims> &matrix) {
    point_xd<Dims> target_sizes{};
    matrix_xd_s<Dims>::get_all_dim_sizes(matrix, target_sizes);
    matrix.emplace(matrix.begin());
    matrix.emplace(matrix.end());
    prepare_matrix_helper<Dims>(matrix, target_sizes);
}

template <size_t Dims, size_t CurrentDim = 0>
void reset_matrix(auto &matrix) {
    for (auto &dim : matrix) {
        if constexpr (CurrentDim == Dims - 2) std::fill(dim.begin(), dim.end(), 0);
        else reset_matrix<Dims, CurrentDim + 1>(dim);
    }
}

template <size_t Dims, size_t CurrentDim = 0>
void perform_cycle_helper(const auto &origin_matrix, auto &current_matrix_dim, auto &matrix_after_cycle, point_xd<Dims> current_point) {
    int64_t start_pos = 0;
    if constexpr (CurrentDim < 2) start_pos = 1;

    for (int64_t pos = start_pos; pos < current_matrix_dim.size() - 1; pos++) {
        current_point[CurrentDim] = pos;
        if constexpr (CurrentDim < Dims - 1) {
            perform_cycle_helper<Dims, CurrentDim + 1>(origin_matrix, current_matrix_dim[pos], matrix_after_cycle[pos], current_point);
        } else {
            auto active_neighbors_count = get_neighbors_sum(origin_matrix, current_point);
            if (active_neighbors_count == 3 || (current_matrix_dim[pos] && active_neighbors_count == 2)) matrix_after_cycle[pos] = true;
        }
    }
}

template <size_t Dims>
void perform_cycle(auto &matrix) {
    prepare_matrix<Dims>(matrix);
    auto matrix_after_cycle = matrix;
    reset_matrix<Dims>(matrix_after_cycle);

    perform_cycle_helper<Dims>(matrix, matrix, matrix_after_cycle, {});

    matrix = matrix_after_cycle;
}

/*void print_matrix(const matrix_4d_t &matrix) {
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
}*/

template <size_t Dims>
size_t compute_matrix_sum(matrix_xd_s_t<Dims> &matrix) {
    matrix_xd_s<Dims>::compute_negative_dimensions(matrix);

    return matrix_xd_s<Dims>::get_matrix_sum(matrix) * 2 - matrix_xd_s<Dims>::get_matrix_zero_dims_sum(matrix);
}

int first_part_2020_17() {
    matrix_xd_s_t<3> matrix;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (std::string str) mutable {
        auto &current_x = matrix.emplace_back();
        for (char c : str) {
            current_x.emplace_back().emplace_back() = c == '#';
        }
    });
    prepare_matrix<3>(matrix); // Add padding

    for (size_t i = 0; i < 6; i++)
        perform_cycle<3>(matrix);

    std::cout << compute_matrix_sum<3>(matrix) << std::endl;
    return EXIT_SUCCESS;
}

int second_part_2020_17() {
    matrix_xd_s_t<4> matrix;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (std::string str) mutable {
        auto &current_x = matrix.emplace_back();
        for (char c : str) {
            current_x.emplace_back().emplace_back().emplace_back() = c == '#';
        }
    });
    prepare_matrix<4>(matrix); // Add padding

    for (size_t i = 0; i < 6; i++) {
        perform_cycle<4>(matrix);
    }

    std::cout << compute_matrix_sum<4>(matrix) << std::endl;
    return EXIT_SUCCESS;
}

#endif