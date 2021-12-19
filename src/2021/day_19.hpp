#ifndef AOC_2021_19
#define AOC_2021_19

#include "../tools/base_includes.h"
#include <stack>
#include <unordered_set>

using scanner_data_t = std::vector<std::pair<std::tuple<int64_t, int64_t, int64_t>, bool>>;

double distance_calc(std::tuple<int64_t, int64_t, int64_t> a, std::tuple<int64_t, int64_t, int64_t> b) {
    return std::pow(std::get<0>(a) - std::get<0>(b), 2) + std::pow(std::get<1>(a) - std::get<1>(b), 2)
           + std::pow(std::get<1>(a) - std::get<1>(b), 2);
}

bool mark_duplicates_between_two_scanners(scanner_data_t &s1, scanner_data_t &s2) {
    std::map<std::tuple<int64_t, int64_t, int64_t>, size_t> ratio_counter;
    for (auto &s1_data: s1) {
        for (auto &s2_data: s2) {
            for (auto x2_sign = -1; x2_sign <= 1; x2_sign += 2) {
                for (auto y2_sign = -1; y2_sign <= 1; y2_sign += 2) {
                    for (auto z2_sign = -1; z2_sign <= 1; z2_sign += 2) {
                        auto x_ratio = std::get<0>(s1_data.first) - x2_sign * std::get<0>(s2_data.first);
                        auto y_ratio = std::get<1>(s1_data.first) - y2_sign * std::get<1>(s2_data.first);
                        auto z_ratio = std::get<2>(s1_data.first) - z2_sign * std::get<2>(s2_data.first);
                        ratio_counter[{x_ratio, y_ratio, z_ratio}]++;

                        x_ratio = std::get<0>(s1_data.first) - x2_sign * std::get<0>(s2_data.first);
                        y_ratio = std::get<1>(s1_data.first) - y2_sign * std::get<2>(s2_data.first);
                        z_ratio = std::get<2>(s1_data.first) - z2_sign * std::get<1>(s2_data.first);
                        ratio_counter[{x_ratio, y_ratio, z_ratio}]++;

                        x_ratio = std::get<0>(s1_data.first) - x2_sign * std::get<1>(s2_data.first);
                        y_ratio = std::get<1>(s1_data.first) - y2_sign * std::get<0>(s2_data.first);
                        z_ratio = std::get<2>(s1_data.first) - z2_sign * std::get<2>(s2_data.first);
                        ratio_counter[{x_ratio, y_ratio, z_ratio}]++;

                        x_ratio = std::get<0>(s1_data.first) - x2_sign * std::get<1>(s2_data.first);
                        y_ratio = std::get<1>(s1_data.first) - y2_sign * std::get<2>(s2_data.first);
                        z_ratio = std::get<2>(s1_data.first) - z2_sign * std::get<0>(s2_data.first);
                        ratio_counter[{x_ratio, y_ratio, z_ratio}]++;

                        x_ratio = std::get<0>(s1_data.first) - x2_sign * std::get<2>(s2_data.first);
                        y_ratio = std::get<1>(s1_data.first) - y2_sign * std::get<0>(s2_data.first);
                        z_ratio = std::get<2>(s1_data.first) - z2_sign * std::get<1>(s2_data.first);
                        ratio_counter[{x_ratio, y_ratio, z_ratio}]++;

                        x_ratio = std::get<0>(s1_data.first) - x2_sign * std::get<2>(s2_data.first);
                        y_ratio = std::get<1>(s1_data.first) - y2_sign * std::get<1>(s2_data.first);
                        z_ratio = std::get<2>(s1_data.first) - z2_sign * std::get<0>(s2_data.first);
                        ratio_counter[{x_ratio, y_ratio, z_ratio}]++;
                    }
                }
            }
        }
    }

    size_t max = 0;
    std::tuple<int64_t, int64_t, int64_t> ratio;
    for (auto counter : ratio_counter) {
        if (counter.second >= 12) {
            max = counter.second;
            ratio = counter.first;
            break;
        }
    }

    size_t marked_counter = 0;
    if (max >= 12) {
        for (auto &s1_data: s1) {
            for (auto &s2_data: s2) {
                for (auto x2_sign = -1; x2_sign <= 1; x2_sign += 2) {
                    for (auto y2_sign = -1; y2_sign <= 1; y2_sign += 2) {
                        for (auto z2_sign = -1; z2_sign <= 1; z2_sign += 2) {
                            auto x_ratio = std::get<0>(s1_data.first) - x2_sign * std::get<0>(s2_data.first);
                            auto y_ratio = std::get<1>(s1_data.first) - y2_sign * std::get<1>(s2_data.first);
                            auto z_ratio = std::get<2>(s1_data.first) - z2_sign * std::get<2>(s2_data.first);
                            if (ratio == std::tuple<int64_t, int64_t, int64_t>{x_ratio, y_ratio, z_ratio}) {
                                if (s2_data.second) s1_data.second = true;
                                s2_data.second = true;
                                marked_counter++;
                            }

                            x_ratio = std::get<0>(s1_data.first) - x2_sign * std::get<0>(s2_data.first);
                            y_ratio = std::get<1>(s1_data.first) - y2_sign * std::get<2>(s2_data.first);
                            z_ratio = std::get<2>(s1_data.first) - z2_sign * std::get<1>(s2_data.first);
                            if (ratio == std::tuple<int64_t, int64_t, int64_t>{x_ratio, y_ratio, z_ratio}) {
                                if (s2_data.second) s1_data.second = true;
                                s2_data.second = true;
                                marked_counter++;
                            }

                            x_ratio = std::get<0>(s1_data.first) - x2_sign * std::get<1>(s2_data.first);
                            y_ratio = std::get<1>(s1_data.first) - y2_sign * std::get<0>(s2_data.first);
                            z_ratio = std::get<2>(s1_data.first) - z2_sign * std::get<2>(s2_data.first);
                            if (ratio == std::tuple<int64_t, int64_t, int64_t>{x_ratio, y_ratio, z_ratio}) {
                                if (s2_data.second) s1_data.second = true;
                                s2_data.second = true;
                                marked_counter++;
                            }

                            x_ratio = std::get<0>(s1_data.first) - x2_sign * std::get<1>(s2_data.first);
                            y_ratio = std::get<1>(s1_data.first) - y2_sign * std::get<2>(s2_data.first);
                            z_ratio = std::get<2>(s1_data.first) - z2_sign * std::get<0>(s2_data.first);
                            if (ratio == std::tuple<int64_t, int64_t, int64_t>{x_ratio, y_ratio, z_ratio}) {
                                if (s2_data.second) s1_data.second = true;
                                s2_data.second = true;
                                marked_counter++;
                            }

                            x_ratio = std::get<0>(s1_data.first) - x2_sign * std::get<2>(s2_data.first);
                            y_ratio = std::get<1>(s1_data.first) - y2_sign * std::get<0>(s2_data.first);
                            z_ratio = std::get<2>(s1_data.first) - z2_sign * std::get<1>(s2_data.first);
                            if (ratio == std::tuple<int64_t, int64_t, int64_t>{x_ratio, y_ratio, z_ratio}) {
                                if (s2_data.second) s1_data.second = true;
                                s2_data.second = true;
                                marked_counter++;
                            }

                            x_ratio = std::get<0>(s1_data.first) - x2_sign * std::get<2>(s2_data.first);
                            y_ratio = std::get<1>(s1_data.first) - y2_sign * std::get<1>(s2_data.first);
                            z_ratio = std::get<2>(s1_data.first) - z2_sign * std::get<0>(s2_data.first);
                            if (ratio == std::tuple<int64_t, int64_t, int64_t>{x_ratio, y_ratio, z_ratio}) {
                                if (s2_data.second) s1_data.second = true;
                                s2_data.second = true;
                                marked_counter++;
                            }
                        }
                    }
                }
            }
        }
    }

    if (max) {
        std::cout << "max: " << max << " mark_counter: " << marked_counter << " ratio: " << std::get<0>(ratio) << " " << std::get<1>(ratio) << " " << std::get<2>(ratio) << std::endl;
        return true;
    }
    return false;
}

void mark_duplicates_between_all_scanners(std::map<size_t, scanner_data_t> &scanners) {
    std::unordered_set<size_t> next_scanners = {0};
    std::set<size_t> visited_scanners;
    while (!next_scanners.empty()) {
        std::unordered_set<size_t> new_next_scanners;
        for (size_t current_scanner : next_scanners) {
            visited_scanners.insert(current_scanner);
            for (auto &s1 : scanners) {
                if (visited_scanners.find(s1.first) != visited_scanners.end()) continue;
                //std::cout << "Compare: " << current_scanner << " and " << s1.first << std::endl;
                if (mark_duplicates_between_two_scanners(scanners.at(current_scanner), s1.second)) {
                    new_next_scanners.insert(s1.first);
                }
            }
        }
        next_scanners = new_next_scanners;
    }
    std::cout << visited_scanners.size() << " " << scanners.size() << std::endl;
}

size_t count_all_detected_elements(std::map<size_t, scanner_data_t> &scanners) {
    size_t elements_count = 0;
    mark_duplicates_between_all_scanners(scanners);
    for (auto &s : scanners) {
        elements_count += std::count_if(s.second.begin(), s.second.end(), [] (const std::pair<std::tuple<int64_t, int64_t, int64_t>, bool> &elem) {
            return !elem.second;
        });
    }
    return elements_count;
}

int first_part_2021_19() {
    //       scanner
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
        data[current_scanner].emplace_back(std::pair<std::tuple<int64_t, int64_t, int64_t>, bool>{{x, y, z}, false});
    });

    std::cout << count_all_detected_elements(data) << std::endl;

    return EXIT_SUCCESS;
}

#endif

// 375 -> too high
// 310 -> not right
// 311 -> not right
// 319 -> not right
// 320 -> not right
// 268 -> too low