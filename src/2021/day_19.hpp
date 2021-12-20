#ifndef AOC_2021_19
#define AOC_2021_19

#include "../tools/base_includes.h"
#include <stack>
#include <unordered_set>

struct point {
    int x;
    int y;
    int z;

    void perform_transformation_fix(int x_sign, int y_sign, int z_sign, int x_ratio, int y_ratio, int z_ratio, int actual_x_location, int actual_y_location, int actual_z_location) {
        int new_x = x_sign * at(actual_x_location) + x_ratio;
        int new_y = y_sign * at(actual_y_location) + y_ratio;
        int new_z = z_sign * at(actual_z_location) + z_ratio;
        x = new_x;
        y = new_y;
        z = new_z;
    }

    int at(size_t place) {
        switch (place) {
            case 0:
                return x;

            case 1:
                return y;

            case 2:
                return z;
        }
        throw std::runtime_error("Wrong idx");
    }

    bool operator<(const point &ref) const {
        return std::tie(x, y, z) < std::tie(ref.x, ref.y, ref.z);
    }

    bool operator==(const point& ref) const {
        return std::tie(x, y, z) == std::tie(ref.x, ref.y, ref.z);
    }
};

using scanner_data_t = std::vector<std::pair<point, bool>>;

bool mark_duplicates_between_two_scanners(std::set<point> &all_beacons, scanner_data_t &s1, scanner_data_t &s2, point& scanner_position) {
    std::map<std::tuple<int64_t, int64_t, int64_t>, size_t> ratio_counter;

    int actual_x_location, actual_y_location, actual_z_location, correction_x_sign, correction_y_sign, correction_z_sign;
    int actual_x_ratio, actual_y_ratio, actual_z_ratio;
    bool correction_found = false;
    for (auto &s1_data: s1) {
        all_beacons.insert(s1_data.first);
        for (auto &s2_data: s2) {
            if (correction_found) break;
            for (auto x2_sign = -1; x2_sign <= 1 && !correction_found; x2_sign += 2) {
                for (auto y2_sign = -1; y2_sign <= 1 && !correction_found; y2_sign += 2) {
                    for (auto z2_sign = -1; z2_sign <= 1 && !correction_found; z2_sign += 2) {
                        auto x_ratio = s1_data.first.at(0) - x2_sign * s2_data.first.at(0);
                        auto y_ratio = s1_data.first.at(1) - y2_sign * s2_data.first.at(1);
                        auto z_ratio = s1_data.first.at(2) - z2_sign * s2_data.first.at(2);
                        if (++ratio_counter[{x_ratio, y_ratio, z_ratio}] >= 12) {
                            actual_x_location = 0;
                            actual_y_location = 1;
                            actual_z_location = 2;
                            correction_x_sign = x2_sign;
                            correction_y_sign = y2_sign;
                            correction_z_sign = z2_sign;
                            correction_found = true;
                            actual_x_ratio = x_ratio; actual_y_ratio = y_ratio; actual_z_ratio = z_ratio;
                        }

                        x_ratio = s1_data.first.at(0) - x2_sign * s2_data.first.at(0);
                        y_ratio = s1_data.first.at(1) - y2_sign * s2_data.first.at(2);
                        z_ratio = s1_data.first.at(2) - z2_sign * s2_data.first.at(1);
                        if (++ratio_counter[{x_ratio, y_ratio, z_ratio}] >= 12) {
                            actual_x_location = 0;
                            actual_y_location = 2;
                            actual_z_location = 1;
                            correction_x_sign = x2_sign;
                            correction_y_sign = y2_sign;
                            correction_z_sign = z2_sign;
                            correction_found = true;
                            actual_x_ratio = x_ratio; actual_y_ratio = y_ratio; actual_z_ratio = z_ratio;
                        }

                        x_ratio = s1_data.first.at(0) - x2_sign * s2_data.first.at(1);
                        y_ratio = s1_data.first.at(1) - y2_sign * s2_data.first.at(0);
                        z_ratio = s1_data.first.at(2) - z2_sign * s2_data.first.at(2);
                        if (++ratio_counter[{x_ratio, y_ratio, z_ratio}] >= 12) {
                            actual_x_location = 1;
                            actual_y_location = 0;
                            actual_z_location = 2;
                            correction_x_sign = x2_sign;
                            correction_y_sign = y2_sign;
                            correction_z_sign = z2_sign;
                            correction_found = true;
                            actual_x_ratio = x_ratio; actual_y_ratio = y_ratio; actual_z_ratio = z_ratio;
                        }

                        x_ratio = s1_data.first.at(0) - x2_sign * s2_data.first.at(1);
                        y_ratio = s1_data.first.at(1) - y2_sign * s2_data.first.at(2);
                        z_ratio = s1_data.first.at(2) - z2_sign * s2_data.first.at(0);
                        if (++ratio_counter[{x_ratio, y_ratio, z_ratio}] >= 12) {
                            actual_x_location = 1;
                            actual_y_location = 2;
                            actual_z_location = 0;
                            correction_x_sign = x2_sign;
                            correction_y_sign = y2_sign;
                            correction_z_sign = z2_sign;
                            correction_found = true;
                            actual_x_ratio = x_ratio; actual_y_ratio = y_ratio; actual_z_ratio = z_ratio;
                        }

                        x_ratio = s1_data.first.at(0) - x2_sign * s2_data.first.at(2);
                        y_ratio = s1_data.first.at(1) - y2_sign * s2_data.first.at(0);
                        z_ratio = s1_data.first.at(2) - z2_sign * s2_data.first.at(1);
                        if (++ratio_counter[{x_ratio, y_ratio, z_ratio}] >= 12) {
                            actual_x_location = 2;
                            actual_y_location = 0;
                            actual_z_location = 1;
                            correction_x_sign = x2_sign;
                            correction_y_sign = y2_sign;
                            correction_z_sign = z2_sign;
                            correction_found = true;
                            actual_x_ratio = x_ratio; actual_y_ratio = y_ratio; actual_z_ratio = z_ratio;
                        }

                        x_ratio = s1_data.first.at(0) - x2_sign * s2_data.first.at(2);
                        y_ratio = s1_data.first.at(1) - y2_sign * s2_data.first.at(1);
                        z_ratio = s1_data.first.at(2) - z2_sign * s2_data.first.at(0);
                        if (++ratio_counter[{x_ratio, y_ratio, z_ratio}] >= 12) {
                            actual_x_location = 2;
                            actual_y_location = 1;
                            actual_z_location = 0;
                            correction_x_sign = x2_sign;
                            correction_y_sign = y2_sign;
                            correction_z_sign = z2_sign;
                            correction_found = true;
                            actual_x_ratio = x_ratio; actual_y_ratio = y_ratio; actual_z_ratio = z_ratio;
                        }
                    }
                }
            }
        }
    }

    if (correction_found) {
        for (auto &s2_data: s2) {
            s2_data.first.perform_transformation_fix(correction_x_sign, correction_y_sign, correction_z_sign,
                                                     actual_x_ratio, actual_y_ratio, actual_z_ratio,
                                                     actual_x_location, actual_y_location, actual_z_location);
            all_beacons.insert(s2_data.first);
        }
        scanner_position = {actual_x_ratio, actual_y_ratio, actual_z_ratio};
    }

    return correction_found;
}

void mark_duplicates_between_all_scanners(std::set<point> &all_beacons, std::map<size_t, scanner_data_t> &scanners) {
    std::unordered_set<size_t> next_scanners = {0};
    std::set<size_t> visited_scanners;
    std::vector<point> positions;
    size_t max_distance;
    while (!next_scanners.empty()) {
        std::unordered_set<size_t> new_next_scanners;
        for (size_t current_scanner : next_scanners) {
            visited_scanners.insert(current_scanner);
            for (auto &s1 : scanners) {
                point scanner_pos{};
                if (visited_scanners.find(s1.first) != visited_scanners.end()) continue;
                if (mark_duplicates_between_two_scanners(all_beacons, scanners.at(current_scanner), s1.second, scanner_pos)) {
                    positions.push_back(scanner_pos);
                    new_next_scanners.insert(s1.first);
                }

            }
        }
        next_scanners = new_next_scanners;
    }

    for (auto &s : positions) {
        for (auto &s1 : positions) {
            auto current_distance = (s.at(0) - s1.at(0)) + (s.at(1) - s1.at(1)) + (s.at(2) - s1.at(2));
            max_distance = std::max((int)max_distance, current_distance);
        }
    }
    std::cout << max_distance << std::endl;
}

size_t count_all_detected_elements(std::map<size_t, scanner_data_t> &scanners) {
    size_t elements_count = 0;
    std::set<point> all_beacons;
    mark_duplicates_between_all_scanners(all_beacons, scanners);
    return all_beacons.size();
}

int first_part_2021_19() {
    std::map<size_t, scanner_data_t> data;
    int current_scanner = -1;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (std::string str) {
        if (str.empty()) return;
        if (str.find("scanner") != std::string::npos) {
            current_scanner++;
            return;
        }
        auto x = std::stoi(str.substr(0, str.find(',')));
        auto y = std::stoi(str.substr(str.find(',') + 1, str.find_last_of(',') - str.find(',') - 1));
        auto z = std::stoi(str.substr(str.find_last_of(',') + 1));
        data[current_scanner].emplace_back(std::pair<point, bool>{point{x, y, z}, false});
    });

    std::cout << count_all_detected_elements(data) << std::endl;

    return EXIT_SUCCESS;
}

#endif