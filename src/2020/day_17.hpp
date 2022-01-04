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
        if constexpr (CurrentDim < Dims) {
            sizes[CurrentDim] = matrix.size();
            get_all_dim_sizes<CurrentDim + 1>(matrix[0], sizes);
        }
    }

    template <size_t TargetDim, size_t CurrentDim = 0>
    static auto get_target_dim_at_zero_point(const auto &matrix) {
        if constexpr (CurrentDim == TargetDim) {
            return matrix;
        } else {
            return get_target_dim_at_zero_point<TargetDim, CurrentDim + 1>(matrix[0]);
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

template <size_t Dims, size_t CurrentDim = 2>
auto& get_matrix_location_in_dims(const auto &matrix, point_xd<Dims> current_dims_point) {
    auto &next_dim_mat = matrix[current_dims_point[CurrentDim]];
    if constexpr (CurrentDim < Dims - 1) {
        return get_matrix_location_in_dims<Dims, CurrentDim + 1>(next_dim_mat, current_dims_point);
    } else return next_dim_mat;
}

template <size_t Dims, size_t CurrentDim = Dims>
void print_matrix(const auto &matrix, point_xd<Dims> current_dims_point = {}) {
    if constexpr (CurrentDim < 3) {
        for (size_t i = 2; i < Dims; i++) {
            std::cout << "Dims" << (i - 2) << " = " << current_dims_point[i] << " ";
        }
        std::cout << "\n";
        std::for_each(matrix.begin(), matrix.end(), [&current_dims_point](const auto &v) {
            std::for_each(v.begin(), v.end(), [&current_dims_point](const auto &v1) {
                auto val_in_dims = get_matrix_location_in_dims<Dims>(v1, current_dims_point);
                std::cout << (val_in_dims ? std::to_string(val_in_dims) : ".") << " ";
            });
            std::cout << std::endl;
        });
    } else {
        auto current_dim_size = matrix_xd_s<Dims>::template get_target_dim_at_zero_point<CurrentDim - 1>(matrix).size();
        for (size_t dim_i = 0; dim_i < current_dim_size; dim_i++) {
            current_dims_point[CurrentDim - 1] = dim_i;
            print_matrix<Dims, CurrentDim - 1>(matrix, current_dims_point);
        }
    }
}

template <size_t Dims>
size_t compute_matrix_sum(matrix_xd_s_t<Dims> &matrix) {
    matrix_xd_s<Dims>::compute_negative_dimensions(matrix);
    return matrix_xd_s<Dims>::get_matrix_sum(matrix) * 2 - matrix_xd_s<Dims>::get_matrix_zero_dims_sum(matrix);
}

template <size_t Dims>
void insert_value_to_dim(auto &place_in_matrix, auto value) {
    if constexpr (Dims < 3) place_in_matrix = value;
    else insert_value_to_dim<Dims - 1>(place_in_matrix.emplace_back(), value);
}

template <size_t Dims>
void perform_day_part() {
    matrix_xd_s_t<Dims> matrix;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (std::string str) mutable {
        auto &current_x = matrix.emplace_back();
        for (char c : str) {
            insert_value_to_dim<Dims>(current_x.emplace_back(), c == '#');
        }
    });
    prepare_matrix<Dims>(matrix); // Add padding

    for (size_t i = 0; i < 6; i++)
        perform_cycle<Dims>(matrix);

    // print_matrix<Dims>(matrix);

    std::cout << compute_matrix_sum<Dims>(matrix) << std::endl;
}

int first_part_2020_17() {
    const size_t dimensions = 3;
    perform_day_part<dimensions>();
    return EXIT_SUCCESS;
}

int second_part_2020_17() {
    const size_t dimensions = 4;
    perform_day_part<dimensions>();
    return EXIT_SUCCESS;
}

#endif