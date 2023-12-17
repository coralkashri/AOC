#ifndef AOC_2023_21
#define AOC_2023_21

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include <array>
#include <set>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

void clear_possibility_from_allergens_h(std::unordered_map<std::string, std::set<std::string>>& allergens_to_possibilities, auto& unknown_ingredients, std::string_view allergen, const std::string& possibility, std::set<std::string>& cleared_allergens) {
    std::vector<std::pair<std::string, std::string>> possibilities_to_clear;
    for (auto& atp : allergens_to_possibilities) {
        if (atp.first != allergen) {
            atp.second.erase(possibility);
            std::erase(unknown_ingredients, possibility);
            if (atp.second.size() == 1 && cleared_allergens.find(atp.first) == cleared_allergens.end()) {
                cleared_allergens.insert(atp.first);
                possibilities_to_clear.emplace_back(atp.first, *atp.second.begin());
            }
        }
    }

    for (const auto& p : possibilities_to_clear) {
        clear_possibility_from_allergens_h(allergens_to_possibilities, unknown_ingredients, p.first, p.second, cleared_allergens);
    }
}

void clear_possibility_from_allergens(std::unordered_map<std::string, std::set<std::string>>& allergens_to_possibilities, auto& unknown_ingredients, std::string_view allergen, const std::string& possibility) {
    std::set<std::string> cleared_allergens;
    clear_possibility_from_allergens_h(allergens_to_possibilities, unknown_ingredients, allergen, possibility, cleared_allergens);
}

int first_part_2020_21() {
    std::unordered_map<std::string, std::set<std::string>> allergens_to_possibilities;
    std::vector<std::string> unknown_ingredients;

    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE),
                  std::istream_iterator<WordDelimitedBy<'\n'>>(),
                  [&allergens_to_possibilities, &unknown_ingredients](std::string str) {
                      std::string_view str_view = str;
                      auto ingredients_str = str_view.substr(0, str_view.find('('));
                      auto allergens_str = str_view.substr(str_view.find("contains") + std::string("contains").size());
                      std::vector<std::string> ingredients;
                      std::string current;
                      for (char c : ingredients_str) {
                          if (c == ' ') {
                              ingredients.push_back(current);
                              unknown_ingredients.push_back(current);
                              current.clear();
                              continue;
                          }
                          current += c;
                      }
                      current.clear();
                      std::sort(ingredients.begin(),  ingredients.end());

                      std::set<std::string> intersections;
                      for (char c : allergens_str) {
                          if (c == ' ' || c == ')' || c == ',') {
                              if (current.empty()) continue;
                              if (!allergens_to_possibilities[current].empty()) {
                                  auto& atp = allergens_to_possibilities[current];
                                  std::set_intersection(atp.begin(), atp.end(),
                                                        ingredients.begin(),  ingredients.end(), std::inserter(intersections, intersections.begin()));
                              } else {
                                  intersections.insert(ingredients.begin(),  ingredients.end());
                              }
                              allergens_to_possibilities[current] = intersections;
                              intersections.clear();
                              current.clear();
                              continue;
                          }
                          current += c;
                      }
                  });

    for (const auto& possibilities : allergens_to_possibilities) {
        if (possibilities.second.size() == 1) {
            clear_possibility_from_allergens(allergens_to_possibilities, unknown_ingredients, possibilities.first, *possibilities.second.begin());
        }
    }

    std::cout << unknown_ingredients.size() << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2020_21() {
    std::unordered_map<std::string, std::set<std::string>> allergens_to_possibilities;
    std::vector<std::string> unknown_ingredients;

    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE),
                  std::istream_iterator<WordDelimitedBy<'\n'>>(),
                  [&allergens_to_possibilities, &unknown_ingredients](std::string str) {
                      std::string_view str_view = str;
                      auto ingredients_str = str_view.substr(0, str_view.find('('));
                      auto allergens_str = str_view.substr(str_view.find("contains") + std::string("contains").size());
                      std::vector<std::string> ingredients;
                      std::string current;
                      for (char c : ingredients_str) {
                          if (c == ' ') {
                              ingredients.push_back(current);
                              unknown_ingredients.push_back(current);
                              current.clear();
                              continue;
                          }
                          current += c;
                      }
                      current.clear();
                      std::sort(ingredients.begin(),  ingredients.end());

                      std::set<std::string> intersections;
                      for (char c : allergens_str) {
                          if (c == ' ' || c == ')' || c == ',') {
                              if (current.empty()) continue;
                              if (!allergens_to_possibilities[current].empty()) {
                                  auto& atp = allergens_to_possibilities[current];
                                  std::set_intersection(atp.begin(), atp.end(),
                                                        ingredients.begin(),  ingredients.end(), std::inserter(intersections, intersections.begin()));
                              } else {
                                  intersections.insert(ingredients.begin(),  ingredients.end());
                              }
                              allergens_to_possibilities[current] = intersections;
                              intersections.clear();
                              current.clear();
                              continue;
                          }
                          current += c;
                      }
                  });

    std::map<std::string, std::string> dangerous_ingredients_list;
    for (const auto& possibilities : allergens_to_possibilities) {
        if (possibilities.second.size() == 1) {
            clear_possibility_from_allergens(allergens_to_possibilities, unknown_ingredients, possibilities.first, *possibilities.second.begin());
        }
    }

    for (const auto& possibilities : allergens_to_possibilities) {
        if (possibilities.second.size() == 1) {
            dangerous_ingredients_list[possibilities.first] = *possibilities.second.begin();
        }
    }

    for (auto& ingredient : dangerous_ingredients_list) {
        std::cout << ingredient.second << ",";
    }

    std::cout << std::endl;
    return EXIT_SUCCESS;
}

#endif