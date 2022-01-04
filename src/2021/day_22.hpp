/* Work In Progress */

#ifndef AOC_2021_22
#define AOC_2021_22

#include "../tools/base_includes.h"
#include <stack>
#include <unordered_set>
#include <atomic>
#include <thread>

std::vector<std::vector<std::vector<int>>> matrix;

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

    range get_common_range(const range &ref) const {
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

    std::vector<range> get_unique_ranges(const range &ref) const {
        std::vector<range> res;
        try {
            auto common = get_common_range(ref);

            if (common.start > start) { // this_start.....common_start
                res.push_back({start, common.start});
            }

            if (common.stop < stop) { // common_stop.....this_stop
                res.push_back({common.stop, stop});
            }
        } catch (...) {
            res.push_back({start, stop});
        }
        return res;
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

    void update_cube_in_matrix() {
        for (int x_pos = x.start; x_pos <= x.stop; x_pos++) {
            for (int y_pos = y.start; y_pos <= y.stop; y_pos++) {
                for (int z_pos = z.start; z_pos <= z.stop; z_pos++) {
                    matrix[x_pos + 10][y_pos + 10][z_pos + 10]++;
                }
            }
        }
    }

    cube get_common_cube(const cube &ref) const {
        auto x_common_range = x.get_common_range(ref.x);
        auto y_common_range = y.get_common_range(ref.y);
        auto z_common_range = z.get_common_range(ref.z);
        if (auto c = cube{x_common_range, y_common_range, z_common_range}; c.size() == 0) throw std::runtime_error("Not in range");
        else return c;
    }

    std::vector<cube> get_unique_cubes(const cube &ref) const {
        std::vector<cube> res;
        auto x_unique_ranges = x.get_unique_ranges(ref.x);
        auto y_unique_ranges = y.get_unique_ranges(ref.y);
        auto z_unique_ranges = z.get_unique_ranges(ref.z);

        auto [is_unique_x_range_exists, is_unique_y_range_exists, is_unique_z_range_exists] = std::tuple{true, true, true};

        if (x_unique_ranges.empty() && y_unique_ranges.empty() && z_unique_ranges.empty()) return res;
        if (x_unique_ranges.empty()) {
            x_unique_ranges.push_back(x);
            is_unique_x_range_exists = false;
        }
        if (y_unique_ranges.empty()) {
            y_unique_ranges.push_back(y);
            is_unique_y_range_exists = false;
        }
        if (z_unique_ranges.empty()) {
            z_unique_ranges.push_back(z);
            is_unique_z_range_exists = false;
        }

        for (auto &unique_x : x_unique_ranges) {
            for (auto &unique_y : y_unique_ranges) {
                for (auto &unique_z : z_unique_ranges) {
                    res.push_back({unique_x, unique_y, unique_z});
                }
            }
        }

        try {
            auto x_common_range = x.get_common_range(x);
            if (is_unique_y_range_exists || is_unique_z_range_exists)
                for (auto &unique_y : y_unique_ranges) {
                    for (auto &unique_z : z_unique_ranges) {
                        res.push_back({x_common_range, unique_y, unique_z});
                    }
                }
        } catch (...) {}

        try {
            auto y_common_range = y.get_common_range(y);
            if (is_unique_x_range_exists || is_unique_z_range_exists)
                for (auto &unique_x : x_unique_ranges) {
                    for (auto &unique_z : z_unique_ranges) {
                        res.push_back({unique_x, y_common_range, unique_z});
                    }
                }
        } catch (...) {}

        try {
            auto z_common_range = z.get_common_range(z);
            if (is_unique_x_range_exists || is_unique_y_range_exists)
                for (auto &unique_x : x_unique_ranges) {
                    for (auto &unique_y : y_unique_ranges) {
                        res.push_back({unique_x, unique_y, z_common_range});
                    }
                }
        } catch (...) {}
        return res;
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
    unsigned long long _size = 0;

    void add_cube(const cube &on_cube) {
        /*std::vector<cube> split_unique_cube;
        //_size += on_cube.size();
        if (on_cubes.empty()) {
            on_cubes.push_back(on_cube);
        }
        *//*else {

            auto c_c = on_cube.get_common_cube(on_cubes[0]);
        }*//*
        for (auto &c : on_cubes) {
            auto uniques = on_cube.get_unique_cubes(c);
            if (!uniques.empty())
                split_unique_cube.insert(split_unique_cube.end(), uniques.begin(), uniques.end());
        }
        if (!split_unique_cube.empty()) {
            on_cubes.insert(on_cubes.end(), split_unique_cube.begin(), split_unique_cube.end());
            for (auto &new_c : split_unique_cube) {
                _size += new_c.size();
            }
        }if (on_cubes.empty()) {
            on_cubes.push_back(on_cube);
            _size += on_cube.size();
        } else {
            std::vector<cube> all_uniques_parts;
            for (auto &c : on_cubes) {
                auto uniques = on_cube.get_unique_cubes(c);
                if (!uniques.empty()) all_uniques_parts.insert(all_uniques_parts.end(), uniques.begin(), uniques.end());
            }
            on_cubes = all_uniques_parts;
            on_cubes.push_back(on_cube);
        }*/
        exclude_cube(on_cube);
        on_cubes.push_back(on_cube);
        _size += on_cube.size();
    }

    void exclude_cube(const cube &off_cube) {
        std::vector<cube> new_on_cubes;
        for (auto &r : on_cubes) {
            try {
                auto common = r.get_common_cube(off_cube);
                _size -= common.size();
                auto uniques = r.get_unique_cubes(off_cube);
                if (!uniques.empty()) new_on_cubes.insert(new_on_cubes.end(), uniques.begin(), uniques.end());
            } catch (...) {}
        }
        on_cubes = new_on_cubes;
    }

    unsigned long long size() {
        unsigned long long actual_size = 0;
        for (size_t i = 0; i < on_cubes.size(); i++) {
            actual_size += on_cubes[i].size();
        }
        return actual_size;
    }

    void print_matrix() {
        matrix = std::vector<std::vector<std::vector<int>>>(21, std::vector<std::vector<int>>(21, std::vector<int>(21, 0)));
        for (cube &c : on_cubes) c.update_cube_in_matrix();
        for (auto &x_row : matrix) {
            for (auto &y_row : x_row) {
                for (auto &z_pos : y_row) {
                    std::cout << z_pos << " ";
                }
                std::cout << "\n";
            }
        }
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

cube parse_input(std::string &&str) {
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

    return {{x_start, x_stop}, {y_start, y_stop}, {z_start, z_stop}};
}

int second_part_2021_22() {
    cubes light_cubes;
    bool current_action;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(), [&] (std::string str) {
        if (str.find(',') == std::string::npos) {
            current_action = str == "on";
            return;
        }

        auto current_cube = parse_input(std::move(str));

        if (current_action) {
            light_cubes.add_cube(current_cube);
        } else {
            //light_cubes.exclude_cube(current_cube);
        }
    });

    std::cout << light_cubes.size() << std::endl;
    //light_cubes.print_matrix();

    return EXIT_SUCCESS;
}

#endif