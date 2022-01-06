#ifndef AOC_2020_7
#define AOC_2020_7

#include "../tools/base_includes.h"

size_t find_bag_options_helper(const std::unordered_map<std::string, std::vector<std::string>> &all_bags_holders, std::set<std::string> &possible_holders, std::string desired_bag) {
    size_t sum = 1;
    if (possible_holders.find(desired_bag) != possible_holders.end()) return 0;
    possible_holders.insert(desired_bag);
    if (all_bags_holders.find(desired_bag) == all_bags_holders.end()) return sum;
    for (auto &optional_holder : all_bags_holders.at(desired_bag)) {
        sum += find_bag_options_helper(all_bags_holders, possible_holders, optional_holder);
    }
    return sum;
}

size_t find_bag_options(const std::unordered_map<std::string, std::vector<std::string>> &all_bags_holders, std::string desired_bag) {
    std::set<std::string> possible_holders;
    return find_bag_options_helper(all_bags_holders, possible_holders, desired_bag) - 1;
}

int first_part_2020_7() {
    std::unordered_map<std::string, std::vector<std::string>> all_bags_holders;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (std::string str) mutable {
        if (str.find("no other bags") != std::string::npos) return;
        std::string container = str.substr(0, str.find(" bags"));
        std::string current_section = ", " + str.substr(str.find("contain") + 8, str.size() - (str.find("contain") + 8));
        while (current_section.find(',') != std::string::npos) {
            current_section = current_section.substr(current_section.find(',') + 4);
            std::string current_contained_bag = current_section.substr(0, current_section.find(" bag"));
            all_bags_holders[current_contained_bag].emplace_back(container);
        }
    });

    std::cout << find_bag_options(all_bags_holders, "shiny gold") << std::endl;

    return EXIT_SUCCESS;
}

/* // There might be cases were the following solution is better than the used one, due to re-computation of the same huge bags.
void update_required_bags(std::unordered_map<std::string, std::vector<std::pair<std::string, size_t>>> &all_bags_requirements, std::string_view target_bag, size_t self_required_bags) {
    for (auto &bag : all_bags_requirements) {
        for (auto &req_bag : bag.second) {
            if (req_bag.first == target_bag) {
                req_bag.second += req_bag.second * self_required_bags;
                break;
            }
        }
    }
}

size_t compute_required_bags_helper(std::unordered_map<std::string, std::vector<std::pair<std::string, size_t>>> &all_bags_requirements, std::set<std::string> &computed_bags, std::string_view target_bag) {
    size_t required_bags_count = 0;
    if (all_bags_requirements.find(target_bag.begin()) == all_bags_requirements.end() || computed_bags.find(target_bag.begin()) != computed_bags.end()) return 0;
    for (auto &required_bag : all_bags_requirements.at(target_bag.begin())) {
        auto required_bags_for_required_bag = compute_required_bags_helper(all_bags_requirements, computed_bags, required_bag.first);
        computed_bags.insert(required_bag.first);
        update_required_bags(all_bags_requirements, required_bag.first, required_bags_for_required_bag);
        required_bags_count += required_bag.second;
    }
    return required_bags_count;
}

size_t compute_required_bags(std::unordered_map<std::string, std::vector<std::pair<std::string, size_t>>> &all_bags_requirements, std::string_view target_bag) {
    std::set<std::string> computed_bags;
    return compute_required_bags_helper(all_bags_requirements, computed_bags, target_bag);
}*/

size_t compute_required_bags(std::unordered_map<std::string, std::vector<std::pair<std::string, size_t>>> &all_bags_requirements, std::string_view target_bag) {
    size_t required_bags_count = 0;
    if (all_bags_requirements.find(target_bag.begin()) == all_bags_requirements.end()) return 0;
    for (auto &required_bag : all_bags_requirements.at(target_bag.begin())) {
        required_bags_count += required_bag.second + required_bag.second * compute_required_bags(all_bags_requirements, required_bag.first);
    }
    return required_bags_count;
}

int second_part_2020_7() {
    std::unordered_map<std::string, std::vector<std::pair<std::string, size_t>>> all_bags_requirements;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (std::string str) mutable {
        if (str.find("no other bags") != std::string::npos) return;
        std::string container = str.substr(0, str.find(" bags"));
        std::string current_section = ", " + str.substr(str.find("contain") + 8, str.size() - (str.find("contain") + 8));
        while (current_section.find(',') != std::string::npos) {
            size_t amount = std::stol(current_section.substr(current_section.find(',') + 2, 1));
            current_section = current_section.substr(current_section.find(',') + 4);
            std::string current_contained_bag = current_section.substr(0, current_section.find(" bag"));
            all_bags_requirements[container].emplace_back(current_contained_bag, amount);
        }
    });

    std::cout << compute_required_bags(all_bags_requirements, "shiny gold") << std::endl;

    return EXIT_SUCCESS;
}

#endif