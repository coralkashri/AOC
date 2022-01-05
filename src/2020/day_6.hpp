#ifndef AOC_2020_6
#define AOC_2020_6

#include "../tools/base_includes.h"

int first_part_2020_6() {
    std::set<char> different_answers_in_group;
    std::cout << std::transform_reduce(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), 0ul, std::plus<>{}, [&] (std::string str) mutable {
        if (str.empty()) {
            auto res = different_answers_in_group.size();
            different_answers_in_group.clear();
            return res;
        }
        different_answers_in_group.insert(str.begin(), str.end());
        return 0ul;
    }) << "\n";

    return EXIT_SUCCESS;
}

int second_part_2020_6() {
    /*// Memory efficient solution:
    std::set<char> all_answers_in_group;
    std::cout << std::transform_reduce(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), 0ul, std::plus<>{}, [&, is_first_line_in_group = true] (std::string str) mutable {
        if (str.empty()) {
            auto res = all_answers_in_group.size();
            all_answers_in_group.clear();
            is_first_line_in_group = true;
            return res;
        }
        if (is_first_line_in_group) {
            all_answers_in_group.insert(str.begin(), str.end());
            is_first_line_in_group = false;
        } else {
            std::set<char> new_common_answers;
            std::sort(str.begin(), str.end());
            std::set_intersection(all_answers_in_group.begin(), all_answers_in_group.end(), str.begin(), str.end(), std::inserter(new_common_answers, new_common_answers.end()));
            std::swap(all_answers_in_group, new_common_answers);
        }
        return 0ul;
    }) << "\n";*/

    // Time efficient solution:
    std::unordered_map<char, size_t> all_answers_in_group = {
            {'a', 0}, {'b', 0}, {'c', 0}, {'d', 0}, {'e', 0}, {'f', 0}, {'g', 0},
            {'h', 0}, {'i', 0}, {'j', 0}, {'k', 0}, {'l', 0}, {'m', 0}, {'n', 0},
            {'o', 0}, {'p', 0}, {'q', 0}, {'r', 0}, {'s', 0}, {'t', 0}, {'u', 0},
            {'v', 0}, {'w', 0}, {'x', 0}, {'y', 0}, {'z', 0}
    };
    std::cout << std::transform_reduce(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), 0ul, std::plus<>{}, [&, current_line = 0] (std::string str) mutable {
        if (str.empty()) {
            size_t res = std::count_if(all_answers_in_group.begin(), all_answers_in_group.end(), [&current_line] (auto &pair) {
                auto res = pair.second == current_line;
                pair.second = 0;
                return res;
            });
            current_line = 0;
            return res;
        }
        for (const auto &c : str) {
            all_answers_in_group[c]++;
        }
        current_line++;
        return 0ul;
    }) << "\n";

    return EXIT_SUCCESS;
}

#endif