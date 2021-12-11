#ifndef AOC_2020_4
#define AOC_2020_4

#include <iostream>
#include <unordered_map>
#include <set>
#include <sstream>
#include <map>
#include <algorithm>
#include <numeric>
#include "../input_source.hpp"
#include "../tools/word_delimited_by.h"
#include "../tools/get_line_stream.h"

bool is_valid_passport(const std::map<std::string, std::string> &passport) {
    return passport.size() == 8 || (passport.size() == 7 && passport.find("cid") == passport.end());
}

int first_part_2020_4() {
    std::stringstream current_line;
    size_t valid_passports;
    std::map<std::string, std::string> exist_fields;
    while (get_line_stream(INPUT_SOURCE, current_line, INPUT_SOURCE.widen('\n'))) {
        if (current_line.str().empty()) { // EOPassport
            valid_passports += is_valid_passport(exist_fields);
            exist_fields.clear();
        } else { // Current passport
            std::stringstream field;
            std::string field_header, field_data;
            while (get_line_stream(current_line, field, ' ')) {
                std::getline(field, field_header, ':');
                std::getline(field, field_data);
                exist_fields[field_header] = field_data;
            }
        }
    }
    valid_passports += is_valid_passport(exist_fields);
    exist_fields.clear();
    std::cout << "Valid passports count: " << valid_passports << std::endl;
    return EXIT_SUCCESS;
}

bool is_all_digits(std::string str) {
    bool res = true;
    for (auto c : str) {
        res = res && c >= '0' && c <= '9';
    }
    return res;
}

bool is_cm(std::string str) {
    return str.size() >= 3 && str.substr(str.size() - 2) == "cm" && is_all_digits(str.substr(0, str.size() - 2));
}

bool is_inch(std::string str) {
    return str.size() >= 3 && str.substr(str.size() - 2) == "in" && is_all_digits(str.substr(0, str.size() - 2));
}

bool is_stringify_number_in_bounds(std::string number, int min_val, int max_val) {
    bool res = is_all_digits(number);
    if (res) {
        auto num = std::stoi(number);
        res = res && num >= min_val && num <= max_val;
    }
    return res;
}

bool is_legal_hgt(std::string str) {
    bool res = true;
    if (is_cm(str)) {
        res = is_stringify_number_in_bounds(str.substr(0, str.size() - 2), 150, 193);
    } else if (is_inch(str)) {
        res = is_stringify_number_in_bounds(str.substr(0, str.size() - 2), 59, 76);
    } else res = false;
    return res;
}

bool is_legal_hcl(std::string str) {
    bool res = str.size() == 7;
    res = res && str.at(0) == '#';
    for (auto c : str.substr(1)) {
        res = res && ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'));
    }
    return res;
}

bool is_legal_ecl(std::string str) {
    return str == "amb" || str == "blu" || str == "brn" || str == "gry" || str == "grn" || str == "hzl" || str == "oth";
}

bool is_strict_valid_passport(const std::map<std::string, std::string> &passport) {
    bool is_valid = passport.size() == 8 || (passport.size() == 7 && passport.find("cid") == passport.end());
    is_valid = is_valid && is_stringify_number_in_bounds(passport.at("byr"), 1920, 2002);
    is_valid = is_valid && is_stringify_number_in_bounds(passport.at("iyr"), 2010, 2020);
    is_valid = is_valid && is_stringify_number_in_bounds(passport.at("eyr"), 2020, 2030);
    is_valid = is_valid && is_legal_hgt(passport.at("hgt"));
    is_valid = is_valid && is_legal_hcl(passport.at("hcl"));
    is_valid = is_valid && is_legal_ecl(passport.at("ecl"));
    is_valid = is_valid && passport.at("pid").size() == 9 && is_all_digits(passport.at("pid"));
    return is_valid;
}

int second_part_2020_4() {
    std::stringstream current_line;
    size_t valid_passports;
    std::map<std::string, std::string> exist_fields;
    while (get_line_stream(INPUT_SOURCE, current_line, INPUT_SOURCE.widen('\n'))) {
        if (current_line.str().empty()) { // EOPassport
            valid_passports += is_strict_valid_passport(exist_fields);
            exist_fields.clear();
        } else { // Current passport
            std::stringstream field;
            std::string field_header, field_data;
            while (get_line_stream(current_line, field, ' ')) {
                std::getline(field, field_header, ':');
                std::getline(field, field_data);
                exist_fields[field_header] = field_data;
            }
        }
    }
    valid_passports += is_strict_valid_passport(exist_fields);
    exist_fields.clear();
    std::cout << "Strict valid passports count: " << valid_passports << std::endl;
    return EXIT_SUCCESS;
}

#endif