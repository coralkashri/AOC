#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>

int first_part() {
    size_t previous_number = 0xFFFFFFFFFFFFFFFF;
    std::cout << std::count_if(std::istream_iterator<std::string>(std::cin), std::istream_iterator<std::string>(), [&] (std::string str) {
        auto current_number = std::stoi(str);
        bool res = current_number > previous_number;
        previous_number = current_number;
        return res;
    }) << std::endl;

    return EXIT_SUCCESS;
}

int second_part() {
    std::vector<size_t> previous_measurements;
    size_t sum;
    std::cout << std::count_if(std::istream_iterator<std::string>(std::cin), std::istream_iterator<std::string>(), [&] (std::string str) {
        bool res = false;
        size_t current_num = std::stoll(str);
        if (previous_measurements.size() < 3) {
            previous_measurements.emplace_back(current_num);
            sum += current_num;
        } else {
            res = sum + current_num - previous_measurements.front() > sum;
            std::rotate(previous_measurements.begin(), previous_measurements.begin() + 1, previous_measurements.end());
            previous_measurements.back() = current_num;
        }
        return res;
    }) << std::endl;

    return EXIT_SUCCESS;
}