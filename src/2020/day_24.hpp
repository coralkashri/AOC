#ifndef AOC_2020_24
#define AOC_2020_24

#include "../tools/base_includes.h"

namespace day_24_tools {

    class floor {
    private:
        using touched_tiles_t = std::unordered_map<point_xd<2>, bool>;

    public:
        floor() : tiles_offsets_by_step({
                                                {"e",  {1,  -1}},
                                                {"se", {0,  -2}},
                                                {"sw", {-1, -1}},
                                                {"w",  {-1, 1}},
                                                {"nw", {0,  2}},
                                                {"ne", {1,  1}},
                                        }) {
            compute_initial_floor_state();
        }

        size_t get_number_of_black_tiles() {
            return std::count_if(touched_tiles.begin(), touched_tiles.end(), [](const auto &pair) { return pair.second; });
        }

        void advanced_days(size_t days) {
            insert_all_neighbors();
            for (size_t i = 0; i < days; i++) advanced_tiles_day();
        }

    private:
        const std::unordered_map<std::string, point_xd<2>> tiles_offsets_by_step;
        touched_tiles_t touched_tiles;

        void update_location(point_xd<2> &current_location, std::string_view next_step) {
            current_location += tiles_offsets_by_step.at({next_step.begin(), next_step.end()});
        }

        auto compute_location(std::string_view all_steps) {
            point_xd<2> target_location{0, 0};
            for (size_t i = 0; i < all_steps.size(); i += 1 + (all_steps[i] != 'e' && all_steps[i] != 'w')) {
                update_location(target_location, {all_steps.begin() + i, all_steps.begin() + i + 1 + (all_steps[i] != 'e' && all_steps[i] != 'w')});
            }
            return target_location;
        }

        void compute_initial_floor_state() {
            std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&](std::string str) mutable {
                auto location = compute_location(str);
                touched_tiles[location] = !touched_tiles[location];
            });
        }

        uint8_t get_black_tiles_neighbors_count(const point_xd<2> &point, touched_tiles_t &new_tiles) const {
            uint8_t res = 0;
            for (auto [symbol, offset] : tiles_offsets_by_step) {
                if (auto it = touched_tiles.find(point + offset); it != touched_tiles.end()) {
                    res += it->second;
                }
                new_tiles[point + offset];
            }
            return res;
        }

        void advanced_tiles_day() {
            touched_tiles_t new_tiles = touched_tiles;
            for (const auto &tile : touched_tiles) {
                auto black_tiles_neighbors_count = get_black_tiles_neighbors_count(tile.first, new_tiles);
                new_tiles[tile.first] = tile.second ? black_tiles_neighbors_count > 0 && black_tiles_neighbors_count <= 2 : black_tiles_neighbors_count == 2;
            }
            touched_tiles = new_tiles;
        }

        void insert_all_neighbors() {
            touched_tiles_t new_tiles = touched_tiles;
            for (const auto &tile : touched_tiles) {
                for (auto [symbol, offset] : tiles_offsets_by_step) {
                    new_tiles[tile.first + offset];
                }
            }
            touched_tiles = new_tiles;
        }
    };
}

int first_part_2020_24() {
    day_24_tools::floor f;

    std::cout << f.get_number_of_black_tiles() << std::endl;
    return EXIT_SUCCESS;
}

int second_part_2020_24() {
    day_24_tools::floor f;

    f.advanced_days(100);

    std::cout << f.get_number_of_black_tiles() << std::endl;
    return EXIT_SUCCESS;
}

#endif