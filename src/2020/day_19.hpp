#ifndef AOC_2020_19
#define AOC_2020_19

#include "../tools/base_includes.h"

enum class input_sections {
    RULES,
    MESSAGES
};

class rule {
public:
    using legal_options_t = std::vector<std::string>;

public:
    rule() : is_final(true) {}

    explicit rule(std::string_view stringify_rule) {
        if (stringify_rule.find('"') != std::string::npos) {
            is_final = true;
            legal_options.emplace_back(std::string() + stringify_rule[1]);
        } else {
            is_final = false;
            legal_options.emplace_back(stringify_rule);
        }
    }

    void set_legal_options(const legal_options_t &new_legal_options) {
        legal_options = new_legal_options;
        is_final = true;
    }

    [[nodiscard]] const auto& get_rule() const {
        return legal_options;
    }

    [[nodiscard]] bool is_parse_required() const {
        return !is_final;
    }

private:
    bool is_final;
    legal_options_t legal_options;
};

class rules {
public:
    void insert_rule(std::string &&stringify_rule) {
        auto rule_number_end = stringify_rule.find(':');
        auto rule_num = std::stoi(stringify_rule.substr(0, rule_number_end));
        rules[rule_num] = rule(stringify_rule.substr(rule_number_end + 2));
    }

    void parse_rules() {
        for (auto &current_rule : rules) {
            if (current_rule.is_parse_required()) {
                current_rule.set_legal_options(parse_rule(current_rule));
            }
        }
    }

    void parse_rule_0() {
        rules[0].set_legal_options(parse_rule(rules[0]));
    }

    [[nodiscard]] bool is_message_legal_by_rule_0(std::string &&message) const {
        auto &rule_0_legal_options = rules[0].get_rule();
        return std::ranges::any_of(rule_0_legal_options, [&message] (const auto &legal_option) {
            return message == legal_option;
        });
    }

    [[nodiscard]] bool is_message_legal_by_loops_rule_0(std::string &&message) const {
        auto copy_message = message;
        auto &rule_42_legal_options = rules[42].get_rule();
        auto should_start_with_42 = copy_message.substr(0, rule_42_legal_options[0].size());
        bool is_legal_start = false;
        for (auto &legal_option : rule_42_legal_options) {
            if (should_start_with_42 == legal_option) is_legal_start = true;
        }
        if (is_legal_start) {
            copy_message = copy_message.substr(rule_42_legal_options[0].size());
            return check_message_polyndrome_of_31_from_end(copy_message) && check_if_rest_message_constructed_with_42_rule(copy_message);
        }
        return false;
    }

private:
    std::array<rule, 200> rules;

    rule::legal_options_t get_all_combinations(std::vector<rule::legal_options_t> &combinations_per_number, size_t current_number = 0) {
        rule::legal_options_t final_possible_combs;
        if (current_number >= combinations_per_number.size()) return final_possible_combs;
        rule::legal_options_t possible_inner_combs = get_all_combinations(combinations_per_number, current_number + 1);
        for (auto &current_number_comb : combinations_per_number[current_number]) {
            if (possible_inner_combs.empty()) {
                final_possible_combs.emplace_back(current_number_comb);
            } else {
                for (auto &possible_comb: possible_inner_combs) {
                    final_possible_combs.emplace_back(current_number_comb + possible_comb);
                }
            }
        }
        return final_possible_combs;
    }

    rule::legal_options_t parse_rule(rule &rule_to_parse) {
        rule::legal_options_t actual_legal_options;
        auto &legal_options = rule_to_parse.get_rule()[0];
        if (legal_options.empty()) return actual_legal_options;
        std::vector<std::string> and_conditions;
        and_conditions.reserve(2);
        if (auto or_sep_location = legal_options.find('|'); or_sep_location != std::string::npos) {
            and_conditions.emplace_back(legal_options.substr(0, or_sep_location));
            and_conditions.emplace_back(legal_options.substr(or_sep_location + 1));
        } else {
            and_conditions.emplace_back(legal_options);
        }
        std::regex number_pattern(R"([0-9]?[0-9]?[0-9])");
        for (auto &current_and_condition : and_conditions) {
            auto numbers_it = std::sregex_iterator(current_and_condition.begin(), current_and_condition.end(), number_pattern);
            std::vector<std::vector<std::string>> combinations_per_number(std::distance(numbers_it, std::sregex_iterator()));
            size_t current_number = 0;
            while (numbers_it != std::sregex_iterator()) {
                auto &current_target_rule = rules[std::stoi(numbers_it->str())];
                rule::legal_options_t new_combinations;
                if (current_target_rule.is_parse_required()) {
                    current_target_rule.set_legal_options(parse_rule(current_target_rule));
                }
                combinations_per_number[current_number++] = current_target_rule.get_rule();
                ++numbers_it;
            }
            auto current_all_and_combinations = get_all_combinations(combinations_per_number);
            actual_legal_options.insert(actual_legal_options.end(), current_all_and_combinations.begin(), current_all_and_combinations.end());
        }
        return actual_legal_options;
    }

    [[nodiscard]] bool check_message_polyndrome_of_31_from_end(std::string &message) const {
        auto &rule_42_legal_options = rules[42].get_rule();
        auto &rule_31_legal_options = rules[31].get_rule();
        int amount_of_31_endings = 0;
        while (!message.empty()) {
            if (!std::ranges::any_of(rule_31_legal_options, [should_end_with_31 = message.substr(message.size() - rule_31_legal_options[0].size())] (const auto &legal_option) {
                return should_end_with_31 == legal_option;
            })) break;
            ++amount_of_31_endings;
            message = message.substr(0, message.size() - rule_31_legal_options[0].size());
        }
        if (!amount_of_31_endings) return false;
        while (!message.empty() && amount_of_31_endings) {
            if (!std::ranges::any_of(rule_42_legal_options, [should_end_with_42 = message.substr(message.size() - rule_42_legal_options[0].size())] (const auto &legal_option) {
                return should_end_with_42 == legal_option;
            })) break;
            --amount_of_31_endings;
            message = message.substr(0, message.size() - rule_42_legal_options[0].size());
        }
        return !amount_of_31_endings;
    }

    [[nodiscard]] bool check_if_rest_message_constructed_with_42_rule(std::string &message) const {
        auto &rule_42_legal_options = rules[42].get_rule();
        while (!message.empty()) {
            if (!std::ranges::any_of(rule_42_legal_options, [should_end_with_42 = message.substr(message.size() - rule_42_legal_options[0].size())] (const auto &legal_option) {
                return should_end_with_42 == legal_option;
            })) return false;
            message = message.substr(0, message.size() - rule_42_legal_options[0].size());
        }
        return true;
    }
};

int first_part_2020_19() {
    rules rs;
    std::list<std::string> all_messages;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&, current_section = input_sections::RULES] (std::string str) mutable {
        switch (current_section) {
            case input_sections::RULES:
                if (str.empty()) {
                    current_section = input_sections::MESSAGES;
                    return;
                }
                rs.insert_rule(std::move(str));
                break;

            case input_sections::MESSAGES:
                all_messages.emplace_back(std::move(str));
                break;
        }
    });
    rs.parse_rule_0();

    std::cout << std::count_if(all_messages.begin(), all_messages.end(), [&] (auto &&message) {
        return rs.is_message_legal_by_rule_0(std::move(message));
    }) << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2020_19() {
    rules rs;
    std::list<std::string> all_messages;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&, current_section = input_sections::RULES] (std::string str) mutable {
        switch (current_section) {
            case input_sections::RULES:
                if (str.empty()) {
                    current_section = input_sections::MESSAGES;
                    return;
                }
                rs.insert_rule(std::move(str));
                break;

            case input_sections::MESSAGES:
                all_messages.emplace_back(std::move(str));
                break;
        }
    });
    rs.parse_rule_0();

    std::cout << std::count_if(all_messages.begin(), all_messages.end(), [&] (auto &&message) {
        return rs.is_message_legal_by_loops_rule_0(std::move(message));
    }) << std::endl;

    return EXIT_SUCCESS;
}

#endif