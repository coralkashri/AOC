#ifndef AOC_2021_20
#define AOC_2021_20

#include "../tools/base_includes.h"
#include <stack>
#include <unordered_set>

using data_t = std::vector<std::vector<int>>;

void print_image(data_t &image) {
    for (auto &row : image) {
        for (auto col : row) {
            std::cout << (col ? '#' : '.');
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

void apply_algo(const std::string &image_enhancement_algo, data_t &image) {
    static size_t iter = 0;
    data_t new_points = image;
    for (int i = 0; i < image.size(); i++) {
        for (int j = 0; j < image[0].size(); j++) {
            size_t num = 0;
            for (int i1 = -1; i1 <= 1; i1++) {
                for (int j1 = -1; j1 <= 1; j1++) {
                    num <<= 1;
                    if (i+i1 < 0 || j+j1 < 0 || i+i1 == image.size() || j+j1==image[0].size()) {
                        num |= (iter % 2);
                    } else {
                        num |= image[i + i1][j + j1];
                    }
                }
            }
            new_points[i][j] = image_enhancement_algo[num] == '#';
        }
    }
    image = new_points;

    print_image(image);
    iter++;
}

int first_part_2021_20() {
    data_t data;
    std::string image_enhancement_algo;
    size_t line_size;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (std::string str) {
        if (str.empty()) return;
        if (image_enhancement_algo.empty()) {
            image_enhancement_algo = str;
            return;
        }
        str = "........................................................" + str + "........................................................";
        data.emplace_back(std::vector<int>(str.size()));
        for (size_t i = 0; i < str.size(); i++) {
            line_size = str.size();
            data.back()[i] = str[i] == '#';
        }
    });
    for (size_t line_offset = 1; line_offset <= 100; line_offset++) {
        data.insert(data.begin(), std::vector<int>(line_size, 0));
        data.insert(data.end(), std::vector<int>(line_size, 0));
    }

    for (size_t i = 0; i < 50; i++) {
        apply_algo(image_enhancement_algo, data);
    }

    std::cout << std::transform_reduce(data.begin(), data.end(), 0, std::plus<>{}, [] (auto &row) {
        return std::count(row.begin(), row.end(), 1);
    }) << std::endl;

    return EXIT_SUCCESS;
}

#endif