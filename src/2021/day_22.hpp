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

    bool operator==(const point &ref) const {
        return std::tie(x, y, z) == std::tie(ref.x, ref.y, ref.z);
    }
};

struct range {
    int64_t start, stop;

    [[nodiscard]] auto get_common_range(const range &ref) const {
        if (ref.stop < start || ref.start > stop) return std::pair{range{}, false};
        /*if (start <= ref.start && ref.stop <= stop) {
            return std::pair{range{ref.start, ref.stop}, true};
        } else if (ref.start <= start && ref.stop <= stop) {
            return std::pair{range{start, ref.stop}, true};
        } else if (start <= ref.start && stop <= ref.stop) {
            return std::pair{range{ref.start, stop}, true};
        } else if (start >= ref.start && stop <= ref.stop) {
            return std::pair{range{start, stop}, true};
        }*/

        return std::pair{range{std::max(start, ref.start), std::min(stop, ref.stop)}, true};
    }

    [[nodiscard]] std::vector<range> get_unique_ranges(const range &ref) const {
        auto[r, is_common_found] = get_common_range(ref);
        if (!is_common_found) return {{start, stop}};

        std::vector<range> uniques;

        if (start < ref.start) uniques.push_back({start, ref.start - 1});

        if (stop > ref.stop) uniques.push_back({ref.stop + 1, stop});

        return uniques;
    }

    uint64_t size() const {
        return stop - start + 1;
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

    [[nodiscard]] std::vector<cube> get_unique_cubes(const cube &ref) const {
        std::vector<cube> res;
        auto x_unique_ranges = x.get_unique_ranges(ref.x);
        auto y_unique_ranges = y.get_unique_ranges(ref.y);
        auto z_unique_ranges = z.get_unique_ranges(ref.z);

        for (auto &unique_x: x_unique_ranges) {
            for (auto &unique_y: y_unique_ranges) {
                for (auto &unique_z: z_unique_ranges) {
                    res.push_back({unique_x, unique_y, unique_z});
                }
            }
        }

        return res;
    }

    [[nodiscard]] uint64_t volume() const {
        return x.size() * y.size() * z.size();
    }
};

struct cubes {
    std::vector<cube> on_cubes;

    void add_cube(const cube &on_cube) {
        exclude_cube(on_cube);
        on_cubes.push_back(on_cube);
    }

    void exclude_cube(const cube &off_cube) {
        std::vector<cube> new_on_cubes;
        for (auto &r: on_cubes) {
            auto uniques = r.get_unique_cubes(off_cube);
            new_on_cubes.insert(new_on_cubes.end(), uniques.begin(), uniques.end());
        }
        on_cubes.swap(new_on_cubes);
        new_on_cubes.clear();
    }

    [[nodiscard]] uint64_t volume() const {
        uint64_t vol = 0;
        for (const auto &on_cube: on_cubes) {
            vol += on_cube.volume();
        }
        return vol;
    }

    void print_matrix() {
        matrix = std::vector<std::vector<std::vector<int>>>(21,
                                                            std::vector<std::vector<int>>(21, std::vector<int>(21, 0)));
        for (cube &c: on_cubes) c.update_cube_in_matrix();
        for (auto &x_row: matrix) {
            for (auto &y_row: x_row) {
                for (auto &z_pos: y_row) {
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
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE), std::istream_iterator<std::string>(),
                  [&](std::string str) {
                      if (str.find(',') == std::string::npos) {
                          current_action = str == "on";
                          return;
                      }
                      auto y_start_pos = str.find(',');
                      auto z_start_pos = str.find_last_of(',');
                      auto x_range = str.substr(0, y_start_pos);
                      auto y_range = str.substr(y_start_pos + 1, z_start_pos - y_start_pos - 1);
                      auto z_range = str.substr(z_start_pos + 1);

                      auto x_start = std::stol(
                              x_range.substr(x_range.find('=') + 1, x_range.find('.') - x_range.find('=') - 1));
                      auto x_stop = std::stol(x_range.substr(x_range.find_last_of('.') + 1));

                      auto y_start = std::stol(
                              y_range.substr(y_range.find('=') + 1, y_range.find('.') - y_range.find('=') - 1));
                      auto y_stop = std::stol(y_range.substr(y_range.find_last_of('.') + 1));

                      auto z_start = std::stol(
                              z_range.substr(z_range.find('=') + 1, z_range.find('.') - z_range.find('=') - 1));
                      auto z_stop = std::stol(z_range.substr(z_range.find_last_of('.') + 1));

                      for (long long x = x_start; x <= x_stop; x++) {
                          for (long long y = y_start; y <= y_stop; y++) {
                              for (long long z = z_start; z <= z_stop; z++) {
                                  matrix[{x, y, z}] = current_action;
                              }
                          }
                      }
                  });

    std::cout << std::count_if(matrix.begin(), matrix.end(), [](auto &point_data) {
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

    return {{x_start, x_stop},
            {y_start, y_stop},
            {z_start, z_stop}};
}

int second_part_2021_22() {
    cubes light_cubes;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(),
                  [&](std::string str) {
                      bool is_on = str.substr(0, str.find(' ')) == "on";

                      auto current_cube = parse_input(str.substr(str.find(' ') + 1));

                      if (is_on) {
                          light_cubes.add_cube(current_cube);
                      } else {
                          light_cubes.exclude_cube(current_cube);
                      }
                  });

    std::cout << light_cubes.volume() << std::endl;

    return EXIT_SUCCESS;
}

#endif