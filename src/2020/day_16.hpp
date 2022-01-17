#ifndef AOC_2020_16
#define AOC_2020_16

#include "../tools/base_includes.h"

enum class input_section {
    RULES,
    SELF_TICKET,
    NEARBY_TICKETS
};

class range {
public:
    range() = default;
    range(uint16_t start, uint16_t stop) : start(start), stop(stop) {}

    [[nodiscard]] bool is_in_range(uint16_t number) const {
        return start <= number && number <= stop;
    }

    std::string get_stringify_rule() const {
        return std::to_string(start) + "-" + std::to_string(stop);
    }

private:
    uint16_t start, stop;
};

class ranges {
public:
    void add_range(range &&r) {
        inner_ranges[current_ranges_count++] = r;
    }

    [[nodiscard]] bool is_in_ranges(uint16_t number) const {
        return inner_ranges[0].is_in_range(number) || inner_ranges[1].is_in_range(number);
    }

    std::string get_stringify_rules() const {
        return inner_ranges[0].get_stringify_rule() + " or " + inner_ranges[1].get_stringify_rule();
    }

private:
    std::array<range, 2> inner_ranges;
    uint8_t current_ranges_count = 0;
};

class ticket_rules_management {
public:
    void parse_rule(std::string &&rule) {
        auto rule_name = rule.substr(0, rule.find(':'));
        auto &current_rule = rules[rule_name];
        std::regex number_pattern(R"([0-9]?[0-9]?[0-9])");
        auto numbers_it = std::sregex_iterator(rule.begin() + rule.find(':'), rule.end(), number_pattern);
        while (numbers_it != std::sregex_iterator()) {
            current_rule.add_range(range(std::stoi(numbers_it->str()), std::stoi((std::next(numbers_it))->str())));
            std::advance(numbers_it, 2);
        }
    }

    std::string find_matched_rule(uint16_t number) const {
        for (auto &rule : rules) {
            if (rule.second.is_in_ranges(number)) return rule.first;
        }
        return "";
    }

    auto find_matched_rules(uint16_t number) const {
        std::set<std::string> matched_rules;
        for (auto &rule : rules) {
            if (rule.second.is_in_ranges(number)) matched_rules.insert(rule.first);
        }
        return matched_rules;
    }

    void print_all_rules() const {
        for (const auto &rule : rules) {
            std::cout << rule.first << ": " << rule.second.get_stringify_rules() << "\n";
        }
    }

private:
    std::unordered_map<std::string, ranges> rules;
};

template <size_t FieldsCount>
class ticket {
private:
    using ticket_inner_data_t = std::array<uint16_t, FieldsCount>;

public:
    explicit ticket(std::string &&stringify_ticket) {
        std::regex number_pattern(R"([0-9]?[0-9]?[0-9])");
        auto numbers_it = std::sregex_iterator(stringify_ticket.begin(), stringify_ticket.end(), number_pattern);
        while (numbers_it != std::sregex_iterator()) {
            numbers[numbers_count++] = std::stoi(numbers_it++->str());
        }
    }

    [[nodiscard]] const ticket_inner_data_t& get_ticket_numbers() const {
        return numbers;
    }

    void print_ticket() const {
        for (auto &num : numbers) std::cout << num << " ";
        std::cout << "\n";
    }

private:
    ticket_inner_data_t numbers;
    uint8_t numbers_count = 0;
};

template <size_t FieldsCount>
class tickets {
private:
    using inner_tickets_t = std::list<ticket<FieldsCount>>;

public:
    void insert_ticket(std::string &&stringify_ticket) {
        inner_tickets.emplace_back(std::move(stringify_ticket));
    }

    size_t fix_ticket_scanning_error_rate() {
        size_t ticket_scanning_error_rate = 0;
        inner_tickets_t correct_tickets;
        bool is_broken_ticket;
        for (auto &t : inner_tickets) {
            auto &current_ticket_numbers = t.get_ticket_numbers();
            is_broken_ticket = false;
            for (auto &num : current_ticket_numbers) {
                if (rules.find_matched_rule(num).empty()) {
                    ticket_scanning_error_rate += num;
                    is_broken_ticket = true;
                    break;
                }
            }
            if (!is_broken_ticket) correct_tickets.insert(correct_tickets.end(), t);
        }
        std::swap(inner_tickets, correct_tickets);
        return ticket_scanning_error_rate;
    }

    void field_names_analysis() {
        uint8_t current_num_ticket_location;
        for (auto &t: inner_tickets) {
            auto &current_ticket_numbers = t.get_ticket_numbers();
            current_num_ticket_location = 0;
            for (auto &num: current_ticket_numbers) {
                auto potential_match_rules = rules.find_matched_rules(num);
                if (field_names[current_num_ticket_location].empty()) { // Initialize location potential rules
                    field_names[current_num_ticket_location] = potential_match_rules;
                } else {
                    typeof(field_names[current_num_ticket_location]) common_potential_match_rules;
                    std::set_intersection(field_names[current_num_ticket_location].begin(), field_names[current_num_ticket_location].end(),
                                          potential_match_rules.begin(), potential_match_rules.end(), std::inserter(common_potential_match_rules, common_potential_match_rules.end()));
                    std::swap(field_names[current_num_ticket_location], common_potential_match_rules);
                    if (field_names[current_num_ticket_location].size() == 1) {
                        remove_known_field_names(*field_names[current_num_ticket_location].begin());
                    }
                }
                ++current_num_ticket_location;
            }
        }
    }

    void remove_known_field_names(const std::string &known_field_name) {
        std::list<std::string> known_field_names(1, known_field_name);
        for (int8_t i = 0; i < field_names.size(); ++i) {
            if (field_names[i].size() == 1) continue;
            for (auto &current_known_field_name : known_field_names)
                field_names[i].erase(current_known_field_name);
            if (field_names[i].size() == 1) {
                known_field_names.template emplace_back(*field_names[i].begin());
                i = -1;
            }
        }
    }

    auto get_departure_fields_from_self_ticket() {
        std::list<uint8_t> target_fields;
        auto &self_ticket_values = inner_tickets.front().get_ticket_numbers();
        for (uint8_t i = 0; i < field_names.size(); ++i)
            if (field_names[i].begin()->find("departure") != std::string::npos) target_fields.template emplace_back(self_ticket_values[i]);
        return target_fields;
    }

    void print_field_names() {
        for (auto &field_name : field_names) {
            if (field_name.size() == 1) std::cout << "Known: " << *field_name.begin() << "\n";
            else std::cout << "Unknown: " << *field_name.begin() << "\n";
        }
        std::cout << "\n";
    }

    void add_tickets_rule(std::string &&rule) {
        rules.parse_rule(std::move(rule));
    }

private:
    inner_tickets_t inner_tickets;
    ticket_rules_management rules;
    std::array<std::set<std::string>, FieldsCount> field_names;
};

template <size_t FieldsCount>
void parse_input(tickets<FieldsCount> &all_tickets) {
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&, current_section = input_section::RULES] (std::string str) mutable {
        switch (current_section) {
            case input_section::RULES:
                if (str.empty()) {
                    current_section = input_section::SELF_TICKET;
                    return;
                }
                all_tickets.add_tickets_rule(std::move(str));
                break;

            case input_section::SELF_TICKET:
                if (str.empty()) {
                    current_section = input_section::NEARBY_TICKETS;
                    return;
                }
                if (str.find(':') != std::string::npos) return;
                all_tickets.insert_ticket(std::move(str));
                break;

            case input_section::NEARBY_TICKETS:
                if (str.find(':') != std::string::npos) return;
                all_tickets.insert_ticket(std::move(str));
                break;
        }
    });
}

int first_part_2020_16() {
    tickets<20> all_tickets;

    parse_input(all_tickets);

    std::cout << all_tickets.fix_ticket_scanning_error_rate() << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2020_16() {
    tickets<20> all_tickets;

    parse_input(all_tickets);

    all_tickets.fix_ticket_scanning_error_rate();

    all_tickets.field_names_analysis();

    auto target_fields = all_tickets.get_departure_fields_from_self_ticket();
    std::cout << std::reduce(target_fields.begin(), target_fields.end(), 1ull, std::multiplies<>{}) << std::endl;

    return EXIT_SUCCESS;
}

#endif