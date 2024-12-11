#ifndef ADVENTOFCODE2024_DAY_8_HPP
#define ADVENTOFCODE2024_DAY_8_HPP

#include <iostream>
#include <algorithm>
#include <iterator>
#include <map>
#include <ranges>
#include <numeric>
#include <regex>

#include "../input_source.hpp"
#include "../tools/str_number.hpp"
#include "../tools/word_delimited_by.h"
#include "../tools/matrix.hpp"
#include "../tools/point.hpp"
#include <set>
#include "../tools/range.hpp"

int first_part_2024_8() {
    std::map<char, std::vector<point_xd<2>>> antennas;
    size_t rows = 0;
    size_t columns = 0;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&antennas, &rows, &columns](std::string str) mutable{
        long long column = 0;
        for (char a : str) {
            if (a != '.') antennas[a].push_back({column, static_cast<long long>(rows)});
            ++column;
        }
        columns = column;
        ++rows;
    });

    std::set<point_xd<2>> antinodes;
    for (const auto& antenna : antennas) {
        const auto& locations = antenna.second;
        for (size_t i = 0; i < locations.size() - 1; ++i) {
            for (size_t j = i + 1; j < locations.size(); ++j) {
                auto diff = locations[i] - locations[j];
                auto antinode = locations[i] + diff;
                if (antinode[0] >= 0 && antinode[0] < columns
                    && antinode[1] >= 0 && antinode[1] < rows)
                    antinodes.insert(antinode);

                antinode = locations[j] - diff;
                if (antinode[0] >= 0 && antinode[0] < columns
                    && antinode[1] >= 0 && antinode[1] < rows)
                    antinodes.insert(antinode);
            }
        }
    }

    std::cout << antinodes.size() << "\n";

    return EXIT_SUCCESS;
}

int second_part_2024_8() {
    std::map<char, std::vector<point_xd<2>>> antennas;
    size_t rows = 0;
    size_t columns = 0;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&antennas, &rows, &columns](std::string str) mutable{
        long long column = 0;
        for (char a : str) {
            if (a != '.') antennas[a].push_back({column, static_cast<long long>(rows)});
            ++column;
        }
        columns = column;
        ++rows;
    });

    std::set<point_xd<2>> antinodes;
    for (const auto& antenna : antennas) {
        const auto& locations = antenna.second;
        for (size_t i = 0; i < locations.size() - 1; ++i) {
            for (size_t j = i + 1; j < locations.size(); ++j) {
                antinodes.insert(locations[i]);
                antinodes.insert(locations[j]);

                auto diff = locations[i] - locations[j];
                auto antinode = locations[i] + diff;
                while (antinode[0] >= 0 && antinode[0] < columns
                    && antinode[1] >= 0 && antinode[1] < rows) {
                    antinodes.insert(antinode);
                    antinode += diff;
                }

                antinode = locations[j] - diff;
                while (antinode[0] >= 0 && antinode[0] < columns
                    && antinode[1] >= 0 && antinode[1] < rows) {
                    antinodes.insert(antinode);
                    antinode -= diff;
                }
            }
        }
    }

    std::cout << antinodes.size() << "\n";

    return EXIT_SUCCESS;
}

#endif
