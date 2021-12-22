#ifndef AOC_2021_22
#define AOC_2021_22

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

struct range {
    long long start, stop;

    range get_both_exists_range(const range &ref) const {
        if (ref.start >= start && ref.stop <= stop) { // ...st...stp...
            return range{ref.start, ref.stop};
        } else if (ref.start <= start && ref.stop <= stop && ref.stop > start) { // st...stp...
            return range{start, ref.stop};
        } else if (ref.start <= start && ref.stop >= stop) { // st......sp
            return range{start, stop};
        } else if (ref.start <= stop && ref.stop >= stop) { // ...st......stp
            return range{ref.start, stop};
        }
        throw std::runtime_error("Not in range");
    }

    unsigned long long size() const {
        return stop - start;
    }

    bool operator==(const range &ref) const {
        return start == ref.start && stop == ref.stop;
    }
};

struct cube {
    range x, y, z;

    cube get_common_cube(const cube &ref) const {
        auto x_common_range = x.get_both_exists_range(ref.x);
        auto y_common_range = y.get_both_exists_range(ref.y);
        auto z_common_range = z.get_both_exists_range(ref.z);
        return {x_common_range, y_common_range, z_common_range};
    }

    bool operator==(const cube &ref) const {
        return x == ref.x && y == ref.y && z == ref.z;
    }

    unsigned long long size() const {
        return x.size() * y.size() * z.size();
    }
};

struct cubes {
    std::vector<cube> on_cubes;
    std::vector<cube> off_cubes;

    void add_range(const cube &on_cube) {
        std::vector<cube> new_on_cubes = on_cubes;
        std::vector<cube> common_excluded_cubes;
        for (auto &r : on_cubes) {
            try {
                auto excluded_cube = r.get_common_cube(on_cube);
                off_cubes.push_back(excluded_cube);
                for (auto &r1 : common_excluded_cubes) {
                    try {
                        auto included_cube = excluded_cube.get_common_cube(r1);
                        new_on_cubes.push_back(included_cube);
                    } catch (...) {}
                }
                common_excluded_cubes.push_back(excluded_cube);
            } catch (...) {}
        }
        on_cubes = new_on_cubes;
        on_cubes.push_back(on_cube);
    }

    void exclude_from_range(const cube &off_cube) {
        for (auto &r : on_cubes) {
            try {
                auto excluded_cube = r.get_common_cube(off_cube);
                off_cubes.push_back(excluded_cube);
            } catch (...) {}
        }
    }

    unsigned long long size() {
        unsigned long long s = 0;
        for (const auto &c : on_cubes) {
            s += c.size();
        }

        for (const auto &r : off_cubes) {
            //std::cout << r.size() << std::endl;
            s -= r.size();
        }
        return s;
    }
};

int first_part_2021_22() {
    std::map<point, bool> matrix;
    bool current_action;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [&] (std::string str) {
        if (str.find(',') == std::string::npos) {
            current_action = str == "on";
            return;
        }
        auto y_start_pos = str.find(',');
        auto z_start_pos = str.find_last_of(',');
        auto x_range = str.substr(0, y_start_pos);
        auto y_range = str.substr(y_start_pos + 1, z_start_pos - y_start_pos - 1);
        auto z_range = str.substr(z_start_pos + 1);

        auto x_start = std::stol(x_range.substr(x_range.find('=') + 1, x_range.find('.') - x_range.find('=') - 1));
        auto x_stop = std::stol(x_range.substr(x_range.find_last_of('.') + 1));

        auto y_start = std::stol(y_range.substr(y_range.find('=') + 1, y_range.find('.') - y_range.find('=') - 1));
        auto y_stop = std::stol(y_range.substr(y_range.find_last_of('.') + 1));

        auto z_start = std::stol(z_range.substr(z_range.find('=') + 1, z_range.find('.') - z_range.find('=') - 1));
        auto z_stop = std::stol(z_range.substr(z_range.find_last_of('.') + 1));

        for (long long x = x_start; x <= x_stop; x++) {
            for (long long  y = y_start; y <= y_stop; y++) {
                for (long long z = z_start; z <= z_stop; z++) {
                    matrix[{x, y, z}] = current_action;
                }
            }
        }
    });

    std::cout << std::count_if(matrix.begin(), matrix.end(), [] (auto &point_data) {
        return point_data.second;
    }) << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2021_22() {
    cubes r;
    std::vector<cube> on_ranges;
    std::vector<cube> off_ranges;
    bool current_action;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [&] (std::string str) {
        if (str.find(',') == std::string::npos) {
            current_action = str == "on";
            return;
        }
        auto y_start_pos = str.find(',');
        auto z_start_pos = str.find_last_of(',');
        auto x_range = str.substr(0, y_start_pos);
        auto y_range = str.substr(y_start_pos + 1, z_start_pos - y_start_pos - 1);
        auto z_range = str.substr(z_start_pos + 1);

        auto x_start = std::stol(x_range.substr(x_range.find('=') + 1, x_range.find('.') - x_range.find('=') - 1));
        auto x_stop = std::stol(x_range.substr(x_range.find_last_of('.') + 1));

        auto y_start = std::stol(y_range.substr(y_range.find('=') + 1, y_range.find('.') - y_range.find('=') - 1));
        auto y_stop = std::stol(y_range.substr(y_range.find_last_of('.') + 1));

        auto z_start = std::stol(z_range.substr(z_range.find('=') + 1, z_range.find('.') - z_range.find('=') - 1));
        auto z_stop = std::stol(z_range.substr(z_range.find_last_of('.') + 1));

        if (current_action) {
            on_ranges.push_back(cube{{x_start, x_stop}, {y_start, y_stop}, {z_start, z_stop}});
        } else {
            off_ranges.push_back(cube{{x_start, x_stop}, {y_start, y_stop}, {z_start, z_stop}});
        }
    });

    for (auto &include_r : on_ranges) {
        r.add_range(include_r);
    }

    for (auto &exclude_r : off_ranges) {
        r.exclude_from_range(exclude_r);
    }

    std::cout << r.size() << std::endl;

    return EXIT_SUCCESS;
}
#endif