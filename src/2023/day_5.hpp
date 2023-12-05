#ifndef AOC_2023_5
#define AOC_2023_5

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

class transformation_details {
public:
    explicit transformation_details(std::string_view transformation_str) {
        str_number<size_t> number;
        unsigned char current_number_index = 0;

        for (char c: transformation_str) {
            if (c == ' ') {
                if (number.is_number_ready()) {
                    numbers[current_number_index++] = number.get_number();
                    number.reset();
                }
                continue;
            }
            number += c;
        }
        numbers[current_number_index++] = number.get_number();
    }

    [[nodiscard]] size_t destination() const { return numbers[0]; }

    [[nodiscard]] size_t source() const { return numbers[1]; }

    [[nodiscard]] size_t size() const { return numbers[2]; }

private:
    std::array<size_t, 3> numbers{};
};

class seeds {
public:
    explicit seeds(std::string_view str) {
        str = str.substr(str.find(':') + 1);

        str_number<size_t> number;
        for (char c: str) {
            if (c == ' ') {
                if (number.is_number_ready()) {
                    current_seeds.emplace_back(number.get_number(), true);
                    number.reset();
                }
                continue;
            }
            number += c;
        }
        current_seeds.emplace_back(number.get_number(), true);
    }

    /**
     * Returns true if all seeds transformed, otherwise returns false
     */
    bool apply_transformation(std::string_view transformation_str) {
        if (transformation_str.empty()) return false;

        if (transformation_str.find(':') != std::string::npos) {
            reset_all_seeds_ready_status();
            return false;
        }

        transformation_details td(transformation_str);
        bool is_all_seeds_ready = true;

        for (auto&[seed, is_transformed]: current_seeds) {
            if (is_transformed) continue;

            if (seed >= td.source() && seed <= td.source() + td.size() - 1) {
                seed = seed - td.source() + td.destination();
                is_transformed = true;
            } else {
                is_all_seeds_ready = false;
            }
        }

        return is_all_seeds_ready;
    }

    size_t get_lowest_seed() {
        size_t lowest_seed = -1;
        for (auto[seed, status]: current_seeds) {
            lowest_seed = std::min(lowest_seed, seed);
        }
        return lowest_seed;
    }

private:
    std::vector<std::pair<size_t, bool>> current_seeds;

    void reset_all_seeds_ready_status() {
        for (auto &seed: current_seeds) {
            seed.second = false;
        }
    }
};

int first_part_2023_5() {
    std::string seeds_str;
    std::getline(INPUT_SOURCE, seeds_str);

    seeds all_seeds(seeds_str);

    std::string transformation_str;
    bool is_all_seeds_ready = false;
    while (std::getline(INPUT_SOURCE, transformation_str)) {
        if (!is_all_seeds_ready || transformation_str.find(':') != std::string::npos) {
            is_all_seeds_ready = all_seeds.apply_transformation(transformation_str);
        }
    }

    std::cout << all_seeds.get_lowest_seed() << std::endl;
    return EXIT_SUCCESS;
}

class range {
private:
    struct size_type {
        size_t size;
    };

    struct stop_type {
        size_t stop;
    };

    struct sub_range {
        explicit sub_range(size_t start, size_type size) :
                start(start), stop(start + size.size - 1), is_transformed(false) {}

        explicit sub_range(size_t start, stop_type stop) :
                start(start), stop(stop.stop), is_transformed(false) {}

        size_t start;
        size_t stop;
        bool is_transformed = false;
    };

public:
    range(size_t start, size_t size) { sub_ranges.emplace_back(start, size_type{size}); }

    void reset_transformed_status() {
        for (auto &r: sub_ranges) {
            r.is_transformed = false;
        }
    }

    void apply_transformation(const transformation_details &td) {
        std::vector<sub_range> new_sub_ranges;
        new_sub_ranges.reserve(2);
        for (auto &r: sub_ranges) {
            if (r.is_transformed) continue;

            if (r.stop < td.source() || r.start > td.source() + td.size() - 1) {
                continue;
            }

            if (r.start >= td.source() && r.stop <= td.source() + td.size() - 1) {
                int64_t ratio = static_cast<int64_t>(td.destination()) - static_cast<int64_t>(td.source());
                r.start = r.start + ratio;
                r.stop = r.stop + ratio;
                r.is_transformed = true;
                continue;
            }

            if (r.start >= td.source()) {
                size_t outside_range_start = td.source() + td.size();
                size_t outside_range_stop = r.stop;
                r.start = r.start - td.source() + td.destination();
                r.stop = td.destination() + td.size() - 1;
                r.is_transformed = true;
                new_sub_ranges.emplace_back(outside_range_start, stop_type{outside_range_stop});
                continue;
            }

            if (r.start < td.source() && r.stop <= td.source() + td.size() - 1) {
                size_t outside_range_start = r.start;
                size_t outside_range_stop = td.source() - 1;
                r.start = td.destination();
                r.stop = r.stop - td.source() + td.destination();
                r.is_transformed = true;
                new_sub_ranges.emplace_back(outside_range_start, stop_type{outside_range_stop});
                continue;
            }

            if (r.start < td.source() && r.stop > td.source() + td.size() - 1) {
                size_t left_start = r.start;
                size_t left_stop = td.source() - 1;

                size_t right_start = td.source() + td.size();
                size_t right_stop = r.stop;

                size_t inside_start = td.destination();
                size_t inside_stop = td.destination() + td.size() - 1;

                r.start = inside_start;
                r.stop = inside_stop;
                r.is_transformed = true;

                new_sub_ranges.emplace_back(left_start, stop_type{left_stop});
                new_sub_ranges.emplace_back(right_start, stop_type{right_stop});
                continue;
            }
        }

        sub_ranges.insert(sub_ranges.end(), new_sub_ranges.begin(), new_sub_ranges.end());
    }

    size_t get_lowest() {
        size_t lowest = -1;
        for (auto &r: sub_ranges) {
            lowest = std::min(lowest, r.start);
        }
        return lowest;
    }

private:
    std::vector<sub_range> sub_ranges;
};

class ranged_seeds {
public:
    explicit ranged_seeds(std::string_view str) {
        str = str.substr(str.find(':') + 1);

        std::array<size_t, 2> current_range{0, 0};
        size_t current_number = 0;
        str_number<size_t> number;
        for (char c: str) {
            if (c == ' ') {
                if (number.is_number_ready()) {
                    current_range[current_number++ % 2] = number.get_number();
                    number.reset();

                    if (current_number % 2 == 0) {
                        current_seeds.emplace_back(current_range[0], current_range[1]);
                    }
                }
                continue;
            }
            number += c;
        }
        current_seeds.emplace_back(current_range[0], number.get_number());
    }

    void apply_transformation(std::string_view transformation_str) {
        if (transformation_str.empty()) return;

        if (transformation_str.find(':') != std::string::npos) {
            reset_all_seeds_ready_status();
            return;
        }

        transformation_details td(transformation_str);
        bool is_all_seeds_ready = true;

        for (auto &seed: current_seeds) {
            seed.apply_transformation(td);
        }
    }

    size_t get_lowest_seed() {
        size_t lowest_seed = -1;

        for (auto &seed: current_seeds) {
            lowest_seed = std::min(lowest_seed, seed.get_lowest());
        }

        return lowest_seed;
    }

private:
    std::vector<range> current_seeds;

    void reset_all_seeds_ready_status() {
        for (auto &seed: current_seeds) {
            seed.reset_transformed_status();
        }
    }
};

int second_part_2023_5() {
    std::string seeds_str;
    std::getline(INPUT_SOURCE, seeds_str);

    ranged_seeds all_seeds(seeds_str);

    std::string transformation_str;
    while (std::getline(INPUT_SOURCE, transformation_str)) {
        all_seeds.apply_transformation(transformation_str);
    }

    std::cout << all_seeds.get_lowest_seed() << std::endl;
    return EXIT_SUCCESS;
}

#endif