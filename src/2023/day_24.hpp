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

    double x = (-ma * xa0 + ya0 - yb0 + mb * xb0) / (mb -ma);
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

int second_part_2023_24() {
    std::vector<hailstone> hailstones = get_hailstones();

    return EXIT_SUCCESS;
}

#endif
