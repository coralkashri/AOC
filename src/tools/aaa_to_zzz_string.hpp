#ifndef ADVENTOFCODE2021_AAA_TO_ZZZ_STRING_HPP
#define ADVENTOFCODE2021_AAA_TO_ZZZ_STRING_HPP

struct aaa_to_zzz_string {
    std::string stringify;

    static uint64_t hash(std::string_view str) {
        uint64_t res = 0;
        int8_t location = 2;
        for (char c: str) {
            res += (c - 'A') * std::pow(26, location--);
        }
        return res;
    }

    static void print_all_possible_combinations() {
        aaa_to_zzz_string my_str{"AAA"};
        for (size_t i = 0; i < 26 * 26 * 26; ++i) {
            std::cout << my_str.stringify << " " << aaa_to_zzz_string::hash(my_str.stringify) << " " << i << std::endl;
            if (i < 26 * 26 * 26 - 1) {
                ++my_str;
            }
        }
    }

    aaa_to_zzz_string &operator++() {
        if (++stringify[2] == 'Z' + 1) {
            stringify[2] = 'A';
            ++stringify[1];
        }

        if (stringify[1] == 'Z' + 1) {
            stringify[1] = 'A';
            ++stringify[0];
        }

        if (stringify[0] == 'Z' + 1) {
            exit(1);
        }

        return *this;
    }
};

#endif //ADVENTOFCODE2021_AAA_TO_ZZZ_STRING_HPP
