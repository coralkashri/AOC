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
            //auto common = get_common_range(ref);
            //if (ref.start - start > 0) res.push_back({start, common.start});
            //if (stop - common.stop > 0) res.push_back({common.stop, stop});

            if (ref.start >= start) {
                if (ref.start - start > 0) res.push_back({start, ref.start});
            } else {
                res.push_back({ref.start, start});
            }

            if (ref.stop >= stop) {
                if (ref.stop - stop > 0) res.push_back({stop, ref.stop});
            } else {
                res.push_back({ref.stop, stop});
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

    cube get_common_cube(const cube &ref) const {
        auto x_common_range = x.get_common_range(ref.x);
        auto y_common_range = y.get_common_range(ref.y);
        auto z_common_range = z.get_common_range(ref.z);
        return {x_common_range, y_common_range, z_common_range};
    }

    std::vector<cube> get_unique_cubes(const cube &ref) const {
        std::vector<cube> res;
        auto x_unique_ranges = x.get_unique_ranges(ref.x);
        auto y_unique_ranges = y.get_unique_ranges(ref.y);
        auto z_unique_ranges = z.get_unique_ranges(ref.z);

        if (x_unique_ranges.empty() && y_unique_ranges.empty() && z_unique_ranges.empty()) return res;
        if (x_unique_ranges.empty()) x_unique_ranges.push_back(x);
        if (y_unique_ranges.empty()) y_unique_ranges.push_back(y);
        if (z_unique_ranges.empty()) z_unique_ranges.push_back(z);

        for (auto &unique_x : x_unique_ranges) {
            for (auto &unique_y : y_unique_ranges) {
                for (auto &unique_z : z_unique_ranges) {
                    res.push_back({unique_x, unique_y, unique_z});
                }
            }
        }
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

    void add_cube(const cube &on_cube) {
        std::vector<cube> split_unique_cube;
        split_unique_cube.push_back(on_cube);
        for (auto &c : on_cubes) {
            std::vector<cube> new_uniques;
            for (auto &split : split_unique_cube) {
                auto uniques = c.get_unique_cubes(split);
                if (!uniques.empty()) new_uniques.insert(new_uniques.end(), uniques.begin(), uniques.end());
            }
            split_unique_cube = new_uniques;
        }
        if (!split_unique_cube.empty()) on_cubes.insert(on_cubes.end(), split_unique_cube.begin(), split_unique_cube.end());
    }

    void exclude_cube(const cube &off_cube) {
        std::vector<cube> new_on_cubes;
        for (auto &r : on_cubes) {
            auto uniques = r.get_unique_cubes(off_cube);
            if (!uniques.empty()) new_on_cubes.insert(new_on_cubes.end(), uniques.begin(), uniques.end());
        }
        //on_cubes = new_on_cubes;
    }

    unsigned long long size() {
        unsigned long long s = 0;
        for (const auto &c : on_cubes) {
            s += c.size();
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
            light_cubes.exclude_cube(current_cube);
        }
    });

    std::cout << light_cubes.size() << std::endl;

    return EXIT_SUCCESS;
}

#endif