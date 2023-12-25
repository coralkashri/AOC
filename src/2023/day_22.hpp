#ifndef AOC_2023_22
#define AOC_2023_22

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

void complete_cubes_fall(auto& cubes) {
    std::sort(cubes.begin(),  cubes.end(), [](const auto& c1, const auto& c2) {
        return c1.first.stop()[2] < c2.first.stop()[2];
    });

    for (int64_t i = 0; i < ssize(cubes); ++i) {
        auto &current_cube = cubes[i];
        auto z_space = current_cube.first.stop()[2] - current_cube.first.start()[2];
        int64_t highest_possible_z = current_cube.first.start()[2];
        int64_t lowest_possible_z = 0;
        current_cube.second = 1;
        for (int64_t j = i - 1; j >= 0; --j) {
            const auto &ref_cube = cubes[j].first;

            if (highest_possible_z <= ref_cube.stop()[2]) continue;

            if (current_cube.first.is_intersect_in_dim(ref_cube, 0)
                && current_cube.first.is_intersect_in_dim(ref_cube, 1)) {

                if (lowest_possible_z < ref_cube.stop()[2] + 1) {
                    lowest_possible_z = ref_cube.stop()[2] + 1;
                    current_cube.second = 1;
                } else if (lowest_possible_z == ref_cube.stop()[2] + 1) ++current_cube.second;
            }
        }

        current_cube.first.start()[2] = lowest_possible_z;
        current_cube.first.stop()[2] = current_cube.first.start()[2] + z_space;
    }
}

void validate_cubes_crossing(const auto& cubes) {
    for (int64_t i = 0; i < ssize(cubes); ++i) {
        const auto &current_cube = cubes[i].first;
        for (int64_t j = 0; j < ssize(cubes); ++j) {
            if (i == j) continue;
            const auto &ref_cube = cubes[j].first;

            if (current_cube.is_crossed(ref_cube)) {
                exit(1);
            }
        }
    }
}

void print_cubes(const auto& cubes) {
    for (const auto& c : cubes) {
        auto& cube = c.first;
        std::cout << cube.locations[0][0] << "," << cube.locations[0][1] << "," << cube.locations[0][2]
                  << "~" << cube.locations[1][0] << "," << cube.locations[1][1] << "," << cube.locations[1][2] << "\n";
    }
}

size_t count_removable_cubes(const auto& cubes) {
    size_t removable_cubes = 0;
    for (int64_t i = 0; i < ssize(cubes); ++i) {
        auto &current_cube = cubes[i].first;
        bool is_free = true;
        for (int64_t j = 0; j < ssize(cubes); ++j) {
            const auto &ref_cube = cubes[j];

            if (ref_cube.first.start()[2] - current_cube.stop()[2] != 1)
                continue;

            if (current_cube.is_intersect_in_dim(ref_cube.first, 0)
                && current_cube.is_intersect_in_dim(ref_cube.first, 1)) {

                if (ref_cube.second == 1) {
                    is_free = false;
                    break;
                }
            }
        }

        removable_cubes += is_free;
    }

    return removable_cubes;
}

int first_part_2023_22() {
    std::vector<std::pair<range_xd<3>, size_t>> cubes;

    std::transform(std::istream_iterator<std::string>(INPUT_SOURCE),
                  std::istream_iterator<std::string>(), std::back_inserter(cubes),
                  [](std::string str) {
        std::string_view str_view = str;
        auto splitter_index = str_view.find('~');
        std::string_view start_view = str_view.substr(0, splitter_index);
        std::string_view stop_view = str_view.substr(splitter_index + 1);
        auto start_numbers = str_numbers<int64_t, std::vector, ','>(start_view).get();
        auto stop_numbers = str_numbers<int64_t, std::vector, ','>(stop_view).get();
        return std::pair{range_xd<3>{
            .locations = {
                    {
                            {start_numbers[0], start_numbers[1], start_numbers[2]},
                            {stop_numbers[0], stop_numbers[1], stop_numbers[2]}
                    }
            }
        }, 0};
    });

    complete_cubes_fall(cubes);

    //validate_cubes_crossing(cubes);

    std::cout << count_removable_cubes(cubes) << std::endl;

    return EXIT_SUCCESS;
}

struct cube {
    range_xd<3> place;
    std::vector<cube*> above;
    std::vector<cube*> below;
    std::vector<cube*> below_copy;
};

void complete_cubes_fall_2(std::vector<cube>& cubes) {
    std::sort(cubes.begin(),  cubes.end(), [](const auto& c1, const auto& c2) {
        return c1.place.stop()[2] < c2.place.stop()[2];
    });

    for (int64_t i = 0; i < ssize(cubes); ++i) {
        auto &current_cube = cubes[i];
        auto z_space = current_cube.place.stop()[2] - current_cube.place.start()[2];
        int64_t highest_possible_z = current_cube.place.start()[2];
        int64_t lowest_possible_z = 0;
        std::vector<cube*> below_cubes;
        for (int64_t j = i - 1; j >= 0; --j) {
            auto &ref_cube = cubes[j];

            if (highest_possible_z <= ref_cube.place.stop()[2]) continue;

            if (current_cube.place.is_intersect_in_dim(ref_cube.place, 0)
                && current_cube.place.is_intersect_in_dim(ref_cube.place, 1)) {

                if (lowest_possible_z < ref_cube.place.stop()[2] + 1) {
                    lowest_possible_z = ref_cube.place.stop()[2] + 1;
                    below_cubes.clear();
                    below_cubes.push_back(&ref_cube);
                } else if (lowest_possible_z == ref_cube.place.stop()[2] + 1) below_cubes.push_back(&ref_cube);;
            }
        }

        current_cube.place.start()[2] = lowest_possible_z;
        current_cube.place.stop()[2] = current_cube.place.start()[2] + z_space;

        for (auto c : below_cubes) {
            c->above.push_back(&current_cube);
        }

        for (auto c : below_cubes) {
            current_cube.below.push_back(&current_cube);
        }
    }
}

void reset_all_cubes_below_copy(std::vector<cube>& cubes) {
    for (auto &c : cubes) {
        c.below_copy = c.below;
    }
}

size_t count_fallen_cubes(cube* current_cube) {
    size_t fallen_cubes = 0;

    std::stack<cube*> cubes_above;
    std::stack<cube*> current_cubes;
    for (auto c : current_cube->above) {
        current_cubes.push(current_cube);
        cubes_above.push(c);
    }

    while (!cubes_above.empty()) {
        cube* above_cube = cubes_above.top();
        cubes_above.pop();

        auto it = std::find(above_cube->below_copy.begin(),  above_cube->below_copy.end(), current_cube);
        above_cube->below_copy.erase(it);

        if (above_cube->below_copy.empty()) {
            ++fallen_cubes;
            current_cube = above_cube;
            for (auto c: current_cube->above) {
                current_cubes.push(current_cube);
                cubes_above.push(c);
            }
        } else {
            current_cube = current_cubes.top();
            current_cubes.pop();
        }
    }

    return fallen_cubes;
}

int second_part_2023_22() {
    std::vector<cube> cubes;

    std::transform(std::istream_iterator<std::string>(INPUT_SOURCE),
                   std::istream_iterator<std::string>(), std::back_inserter(cubes),
                   [](std::string str) {
                       std::string_view str_view = str;
                       auto splitter_index = str_view.find('~');
                       std::string_view start_view = str_view.substr(0, splitter_index);
                       std::string_view stop_view = str_view.substr(splitter_index + 1);
                       auto start_numbers = str_numbers<int64_t, std::vector, ','>(start_view).get();
                       auto stop_numbers = str_numbers<int64_t, std::vector, ','>(stop_view).get();
                       return cube{range_xd<3>{
                               .locations = {
                                       {
                                               {start_numbers[0], start_numbers[1], start_numbers[2]},
                                               {stop_numbers[0], stop_numbers[1], stop_numbers[2]}
                                       }
                               }
                       }, {}};
                   });

    complete_cubes_fall_2(cubes);

    size_t fallen_cubes = 0;
    for (int64_t i = 0; i < ssize(cubes); ++i) {
        auto *current_cube = &cubes[i];
        reset_all_cubes_below_copy(cubes);
        fallen_cubes += count_fallen_cubes(current_cube);
    }

    std::cout << fallen_cubes << std::endl;

    return EXIT_SUCCESS;
}

#endif
