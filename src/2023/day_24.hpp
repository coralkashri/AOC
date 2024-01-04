#ifndef AOC_2023_24
#define AOC_2023_24

/*
 * Math calculations are done by [Ellie Bogdanov](https://www.linkedin.com/in/ellie-bogdanov-java/?originalSubdomain=il)
 */

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include <array>
#include <set>
#include <queue>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

struct hailstone {
    point_xd<3> starting_position;
    point_xd<3> velocity;
};

bool is_collide(const hailstone& a, const hailstone& b, int index) {
    double xa0 = a.starting_position[0];
    double xat = a.velocity[0];
    double ya0 = a.starting_position[1];
    double yat = a.velocity[1];

    double xb0 = b.starting_position[0];
    double xbt = b.velocity[0];
    double yb0 = b.starting_position[1];
    double ybt = b.velocity[1];

    double xa1 = xa0 + xat;
    double ya1 = ya0 + yat;
    double xb1 = xb0 + xbt;
    double yb1 = yb0 + ybt;

    double ma = (ya1 - ya0) / (xa1 - xa0);
    double mb = (yb1 - yb0) / (xb1 - xb0);

    if (mb - ma == 0) return false;

    double test_area_start = USE_TEST_CASE ? 7 : 200000000000000;
    double test_area_stop = USE_TEST_CASE ? 27 : 400000000000000;

    double x = (-ma * xa0 + ya0 - yb0 + mb * xb0) / (mb - ma);
    double y = ma * (x - xa0) + ya0;

    if (!(x >= test_area_start && x <= test_area_stop && y >= test_area_start && y <= test_area_stop)) return false;

    double time_a = (x - xa0) / xat;
    double time_b = (x - xb0) / xbt;

    if (time_a < 0 || time_b < 0) return false;

    return true;
}

std::vector<hailstone> get_hailstones() {
    std::vector<hailstone> hailstones;
    std::transform(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE),
                   std::istream_iterator<WordDelimitedBy<'\n'>>(), std::back_inserter(hailstones),
                   [](std::string str) {
                       std::string_view str_view = str;
                       size_t start_velocity_section = str_view.find('@');
                       std::string_view position = str_view.substr(0, start_velocity_section);
                       std::string_view velocity = str_view.substr(start_velocity_section + 2);

                       auto position_numbers = str_numbers(position).get();
                       auto velocity_numbers = str_numbers(velocity).get();

                       return hailstone{
                               .starting_position = {
                                       position_numbers[0], position_numbers[1], position_numbers[2]
                               },
                               .velocity = {
                                       velocity_numbers[0], velocity_numbers[1], velocity_numbers[2]
                               }
                       };
                   });
    return hailstones;
}

int first_part_2023_24() {
    std::vector<hailstone> hailstones = get_hailstones();

    size_t res = 0;
    int index = 0;
    for (size_t i = 0; i < hailstones.size() - 1; ++i) {
        for (size_t j = i + 1; j < hailstones.size(); ++j) {
            res += is_collide(hailstones[i], hailstones[j], index++);
        }
    }

    std::cout << res << std::endl;

    return EXIT_SUCCESS;
}

struct parameters {
    int64_t parameters[4];
    int64_t res;
};

size_t get_hyperplanes_from_hailstones(const std::vector<hailstone*> &hailstones) {
    std::vector<parameters> matrix;

    for (size_t i = 0; i < hailstones.size() - 1; ++i) {
        size_t j = i + 1;
        int64_t dy = hailstones[i]->velocity[1];
        int64_t y = hailstones[i]->starting_position[1];
        int64_t dy_tag = hailstones[j]->velocity[1];
        int64_t y_tag = hailstones[j]->starting_position[1];

        int64_t dx = hailstones[i]->velocity[0];
        int64_t x = hailstones[i]->starting_position[0];
        int64_t dx_tag = hailstones[j]->velocity[0];
        int64_t x_tag = hailstones[j]->starting_position[0];

        matrix.emplace_back();
        matrix.back().parameters[0] = dy_tag - dy;
        matrix.back().parameters[1] = dx - dx_tag;
        matrix.back().parameters[2] = y - y_tag;
        matrix.back().parameters[3] = x_tag - x;
        matrix.back().res = x_tag * dy_tag - y_tag * dx_tag - x * dy + y * dx;
    }

    std::vector<LinearAlgebra::hyperplane> hyperplanes;

    for (auto[p, res]: matrix) {
        hyperplanes.emplace_back(LinearAlgebra::vector({(double)p[0], (double)p[1], (double)p[2], (double)p[3]}), res);
    }

    int64_t desired_res = 0;

    LinearAlgebra::linear_system s(hyperplanes);
    LinearAlgebra::linear_system::Result res = s.compute_ge_solution(); // (x, y, x_velocity, y_velocity)
    desired_res += std::round(res.res[0]) + std::round(res.res[1]);

    std::cout << res << std::endl;

    std::vector<parameters> z_matrix;

    for (size_t i = 0; i < 2; ++i) {
        size_t j = i + 1;
        int64_t dy = hailstones[i]->velocity[1];
        int64_t y = hailstones[i]->starting_position[1];
        int64_t dy_tag = hailstones[j]->velocity[1];
        int64_t y_tag = hailstones[j]->starting_position[1];

        int64_t dz = hailstones[i]->velocity[2];
        int64_t z = hailstones[i]->starting_position[2];
        int64_t dz_tag = hailstones[j]->velocity[2];
        int64_t z_tag = hailstones[j]->starting_position[2];

        z_matrix.emplace_back();
        z_matrix.back().parameters[0] = (dy_tag - dy);
        z_matrix.back().parameters[1] = (y - y_tag);
        z_matrix.back().res = (z_tag * dy_tag - y_tag * dz_tag - z * dy + y * dz
                               - std::round(res.res[1]) * (dz - dz_tag) - std::round(res.res[3]) * (z_tag - z));
    }

    hyperplanes.clear();

    for (auto[p, res]: z_matrix) {
        hyperplanes.emplace_back(LinearAlgebra::vector({(double)p[0], (double)p[1]}), res);
    }

    s = LinearAlgebra::linear_system(hyperplanes);
    res = s.compute_ge_solution();

    desired_res += std::round(res.res[0]);
    std::cout << res << std::endl;

    return desired_res;
}

int second_part_2023_24() {
    std::vector<hailstone> hailstones = get_hailstones();

    std::cout << std::setprecision(20);

    for (size_t i = 0; i < hailstones.size() - 4; ++i) {
        for (size_t j = i + 1; j < hailstones.size() - 3; ++j) {
            for (size_t k = j + 1; k < hailstones.size() - 2; ++k) {
                for (size_t l = k + 1; l < hailstones.size() - 1; ++l) {
                    for (size_t m = l + 1; m < hailstones.size(); ++m) {
                        try {
                            std::cout << get_hyperplanes_from_hailstones({&hailstones[i],
                                                                          &hailstones[j],
                                                                          &hailstones[k],
                                                                          &hailstones[l],
                                                                          &hailstones[m]}) << std::endl;
                            std::cout << i << " " << j << " " << k << " " << l << " " << m << "\n";
                            return EXIT_SUCCESS;
                        } catch (...) {}
                    }
                }
            }
        }
    }

    return EXIT_SUCCESS;
}

#endif
