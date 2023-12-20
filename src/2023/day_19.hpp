#ifndef AOC_2023_19
#define AOC_2023_19

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

struct group {
    std::map<char, int64_t> numbers;

    int64_t sum() const {
        int64_t res = 0;
        for (auto &p: numbers) res += p.second;
        return res;
    }
};

struct condition {
    char variable;
    bool is_bigger;
    int64_t number;
    std::string target_if_true;

    bool filter(const group &g) const {
        return is_bigger ? g.numbers.at(variable) > number : g.numbers.at(variable) < number;
    }
};

struct conditions {
    std::vector<condition> conds;
    std::string target_if_false;

    std::string get_dest(const group &g) const {
        for (auto &cond: conds) {
            if (cond.filter(g)) return cond.target_if_true;
        }
        return target_if_false;
    }
};

int first_part_2023_19() {
    std::unordered_map<std::string, conditions> paths;
    std::vector<int64_t> accepted;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE),
                  std::istream_iterator<std::string>(),
                  [&paths, &accepted](std::string str) {
                      std::string_view str_view = str;
                      if (str_view[0] != '{') { // path
                          auto conditions_scope_start = str.find('{');
                          std::string source_name = str.substr(0, conditions_scope_start);
                          auto &current_path = paths[source_name];
                          str_view = str_view.substr(conditions_scope_start + 1,
                                                     str_view.size() - conditions_scope_start - 2);
                          auto next_section_start = str_view.find(',');
                          while (next_section_start != std::string::npos) {
                              auto target_split_index = str_view.find(':');
                              condition c{
                                      .variable = str_view[0],
                                      .is_bigger = str_view[1] == '>',
                                      .number = str_number(str_view.substr(2, target_split_index - 2)).get_number(),
                              };
                              c.target_if_true = str_view.substr(target_split_index + 1,
                                                                 next_section_start - target_split_index - 1);
                              current_path.conds.push_back(c);
                              str_view = str_view.substr(next_section_start + 1);
                              next_section_start = str_view.find(',');
                          }
                          current_path.target_if_false = str_view;
                          return;
                      }
                      // action
                      str_view = str_view.substr(1, str_view.size() - 2);
                      auto next_section_start = str_view.find(',');
                      group g;
                      while (next_section_start != std::string::npos) {
                          g.numbers[str_view[0]] = str_number(str_view.substr(2, next_section_start - 2)).get_number();
                          str_view = str_view.substr(next_section_start + 1);
                          next_section_start = str_view.find(',');
                      }
                      g.numbers[str_view[0]] = str_number(str_view.substr(2, next_section_start)).get_number();
                      std::string d;
                      for (d = paths["in"].get_dest(g); d != "A" && d != "R"; d = paths[d].get_dest(g)) {
                          //std::cout << d << " -> ";
                      }

                      if (d == "A") {
                          accepted.push_back(g.sum());
                      }
                  });

    std::cout << std::reduce(accepted.begin(), accepted.end()) << std::endl;

    return EXIT_SUCCESS;
}

size_t get_accepted(const std::unordered_map<std::string, conditions> &paths, const std::string &current_path,
                    size_t current_cond_index = 0,
                    std::pair<int64_t, int64_t> x = {1, 4000},
                    std::pair<int64_t, int64_t> m = {1, 4000},
                    std::pair<int64_t, int64_t> a = {1, 4000},
                    std::pair<int64_t, int64_t> s = {1, 4000}) {
    if (current_path == "R") return 0;
    if (current_path == "A")
        return (x.second - x.first + 1) *
               (m.second - m.first + 1) *
               (a.second - a.first + 1) *
               (s.second - s.first + 1);

    auto &p = paths.at(current_path);
    if (p.conds.size() == current_cond_index) {
        return get_accepted(paths, p.target_if_false, 0, x, m, a, s);
    }

    size_t res = 0;
    auto &c = p.conds[current_cond_index];
    switch (c.variable) {
        case 'x':
            if (c.is_bigger) {
                auto new_start = std::max(x.first, c.number + 1);
                if (new_start > x.second) break;
                res += get_accepted(paths, c.target_if_true, 0, {std::max(x.first, c.number + 1), x.second}, m, a, s);
                return res + get_accepted(paths, current_path, current_cond_index + 1, {x.first, std::max(x.first, c.number + 1) - 1}, m, a, s);
            } else {
                auto new_stop = std::min(x.second, c.number - 1);
                if (new_stop < x.first) break;
                res += get_accepted(paths, c.target_if_true, 0, {x.first, {std::min(x.second, c.number - 1)}}, m, a, s);
                return res + get_accepted(paths, current_path, current_cond_index + 1, {{std::min(x.second, c.number - 1) + 1}, x.second}, m, a, s);
            }
            break;
        case 'm':
            if (c.is_bigger) {
                auto new_start = std::max(m.first, c.number + 1);
                if (new_start > m.second) break;
                res += get_accepted(paths, c.target_if_true, 0, x, {std::max(m.first, c.number + 1), m.second}, a, s);
                return res + get_accepted(paths, current_path, current_cond_index + 1, x, {m.first, std::max(m.first, c.number + 1) - 1}, a, s);
            } else {
                auto new_stop = std::min(m.second, c.number - 1);
                if (new_stop < m.first) break;
                res += get_accepted(paths, c.target_if_true, 0, x, {m.first, {std::min(m.second, c.number - 1)}}, a, s);
                return res + get_accepted(paths, current_path, current_cond_index + 1, x, {{std::min(m.second, c.number - 1) + 1}, m.second}, a, s);
            }
            break;
        case 'a':
            if (c.is_bigger) {
                auto new_start = std::max(a.first, c.number + 1);
                if (new_start > a.second) break;
                res += get_accepted(paths, c.target_if_true, 0, x, m, {std::max(a.first, c.number + 1), a.second}, s);
                return res + get_accepted(paths, current_path, current_cond_index + 1, x, m, {a.first, std::max(a.first, c.number + 1) - 1}, s);
            } else {
                auto new_stop = std::min(a.second, c.number - 1);
                if (new_stop < a.first) break;
                res += get_accepted(paths, c.target_if_true, 0, x, m, {a.first, {std::min(a.second, c.number - 1)}}, s);
                return res + get_accepted(paths, current_path, current_cond_index + 1, x, m, {{std::min(a.second, c.number - 1) + 1}, a.second}, s);
            }
            break;
        case 's':
            if (c.is_bigger) {
                auto new_start = std::max(s.first, c.number + 1);
                if (new_start > s.second) break;
                res += get_accepted(paths, c.target_if_true, 0, x, m, a, {std::max(s.first, c.number + 1), s.second});
                return res + get_accepted(paths, current_path, current_cond_index + 1, x, m, a, {s.first, std::max(s.first, c.number + 1) - 1});
            } else {
                auto new_stop = std::min(s.second, c.number - 1);
                if (new_stop < s.first) break;
                res += get_accepted(paths, c.target_if_true, 0, x, m, a, {s.first, {std::min(s.second, c.number - 1)}});
                return res + get_accepted(paths, current_path, current_cond_index + 1, x, m, a, {{std::min(s.second, c.number - 1) + 1}, s.second});
            }
            break;
    }
}

int second_part_2023_19() {
    std::unordered_map<std::string, conditions> paths;
    std::vector<int64_t> accepted, rejected;
    std::for_each(std::istream_iterator<std::string>(INPUT_SOURCE),
                  std::istream_iterator<std::string>(),
                  [&paths, &accepted](std::string str) {
                      std::string_view str_view = str;
                      if (str_view[0] != '{') { // path
                          auto conditions_scope_start = str.find('{');
                          std::string source_name = str.substr(0, conditions_scope_start);
                          auto &current_path = paths[source_name];
                          str_view = str_view.substr(conditions_scope_start + 1,
                                                     str_view.size() - conditions_scope_start - 2);
                          auto next_section_start = str_view.find(',');
                          while (next_section_start != std::string::npos) {
                              auto target_split_index = str_view.find(':');
                              condition c{
                                      .variable = str_view[0],
                                      .is_bigger = str_view[1] == '>',
                                      .number = str_number(str_view.substr(2, target_split_index - 2)).get_number(),
                              };
                              c.target_if_true = str_view.substr(target_split_index + 1,
                                                                 next_section_start - target_split_index - 1);
                              current_path.conds.push_back(c);
                              str_view = str_view.substr(next_section_start + 1);
                              next_section_start = str_view.find(',');
                          }
                          current_path.target_if_false = str_view;
                          return;
                      }
                      // action
                      return;
                  });

    std::cout << get_accepted(paths, "in") << std::endl;

    return EXIT_SUCCESS;
}

#endif
