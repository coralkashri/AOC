#ifndef AOC_2021_20
#define AOC_2021_20

#include "../tools/base_includes.h"
#include <stack>
#include <unordered_set>

void print_image(std::map<std::pair<long long, long long>, bool> &image) {
    int current_line = -1000;
    for (auto &p : image) {
        if (p.first.first != current_line) {
            current_line = p.first.first;
            std::cout << std::endl;
        }
        std::cout << (p.second ? '#' : '.');
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

void apply_algo(const std::string &image_enhancement_algo, std::map<std::pair<long long, long long>, bool> &image) {
    auto copy_image = image;
    std::map<std::pair<long long, long long>, bool> new_points;
    for (auto &point : image) {
        size_t num = 0;
        auto current_point = point.first;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                num <<= 1;
                auto offset_point = std::pair{current_point.first + i, current_point.second + j};
                if (copy_image.find(offset_point) != copy_image.end()) {
                    num |= copy_image[{current_point.first + i, current_point.second + j}];
                } else {
                    num |= true;
                }
            }
        }
        new_points[current_point] = image_enhancement_algo[num] == '#';
    }
    image = copy_image;
    for (auto &point : new_points) {
        image[point.first] = point.second;
    }

    print_image(image);
}

int first_part_2021_20() {
    std::map<std::pair<long long, long long>, bool> data;
    std::string image_enhancement_algo;
    size_t current_line = 0;
    size_t line_size;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (std::string str) {
        if (str.empty()) return;
        if (image_enhancement_algo.empty()) {
            image_enhancement_algo = str;
            return;
        }
        //str = "." + str + ".";
        for (size_t i = 0; i < str.size(); i++) {
            line_size = str.size();
            data[{current_line, i}] = str[i] == '#';
        }
        current_line++;
    });
    /*for (size_t line_offset = 1; line_offset <= 1; line_offset++) {
        for (size_t i = 0; i < line_size; i++) {
            data[{-1 * line_offset, i}] = false;
            data[{current_line + line_offset - 1, i}] = false;
        }
    }*/

    for (size_t i = 0; i < 2; i++) {
        apply_algo(image_enhancement_algo, data);
    }

    std::cout << std::count_if(data.begin(), data.end(), [] (auto &point) {
        return point.second;
    }) << std::endl;

    return EXIT_SUCCESS;
}

#endif

// 5573 -> too high

// 5357 -> too low
// 5350 -> too low