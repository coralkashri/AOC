#ifndef AOC_2020_24
#define AOC_2020_24

#include "../tools/base_includes.h"

namespace day_24_tools {

    class floor {
    private:
        using touched_tiles_t = std::vector<std::vector<uint8_t>>;
        const size_t floor_rows_count = 300, floor_lines_count = 300;

    public:
        floor() : tiles_offsets_by_step({
                                                {"e",  {1,  -1}},
                                                {"se", {0,  -2}},
                                                {"sw", {-1, -1}},
                                                {"w",  {-1, 1}},
                                                {"nw", {0,  2}},
                                                {"ne", {1,  1}},
                                                {"self", {0,  0}},
                                        }),
                  all_tiles(std::vector<std::vector<uint8_t>>(floor_rows_count, std::vector<uint8_t>(floor_lines_count, false))) {
            compute_initial_floor_state();
        }

        size_t get_number_of_black_tiles() {
            return black_tiles.size();
        }

        void advanced_days(size_t days) {
            for (size_t i = 0; i < days; i++) advanced_tiles_day();
        }

    private:
        const std::unordered_map<std::string, point_xd<2>> tiles_offsets_by_step;
        touched_tiles_t all_tiles;
        std::list<point_xd<2>> black_tiles;

        void update_location(point_xd<2> &current_location, std::string_view next_step) {
            current_location += tiles_offsets_by_step.at({next_step.begin(), next_step.end()});
        }

        auto compute_location(std::string_view all_steps) {
            point_xd<2> target_location{(int64_t)all_tiles.size() / 2, (int64_t)all_tiles[0].size() / 2};
            for (size_t i = 0; i < all_steps.size(); i += 1 + (all_steps[i] != 'e' && all_steps[i] != 'w')) {
                update_location(target_location, {all_steps.begin() + i, all_steps.begin() + i + 1 + (all_steps[i] != 'e' && all_steps[i] != 'w')});
            }
            return target_location;
        }

        void compute_initial_floor_state() {
            std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&](std::string str) mutable {
                auto location = compute_location(str);
                all_tiles[location.places[0]][location.places[1]] = !all_tiles[location.places[0]][location.places[1]];
                if (all_tiles[location.places[0]][location.places[1]]) black_tiles.insert(black_tiles.end(), {location.places[0], location.places[1]});
                else std::erase(black_tiles, point_xd<2>{location.places[0], location.places[1]});
            });
        }

        uint8_t get_black_tiles_neighbors_count(const point_xd<2> &point) const {
            uint8_t res = 0;
            for (auto [symbol, offset] : tiles_offsets_by_step) {
                auto location = point + offset;
                res += all_tiles[location.places[0]][location.places[1]];
            }
            res -= all_tiles[point.places[0]][point.places[1]];
            return res;
        }

        void advanced_tiles_day() {
            touched_tiles_t new_tiles = all_tiles;
            std::list<point_xd<2>> new_black_tiles;
            std::unordered_set<point_xd<2>> visited_locations;
            for (const auto &black_tile : black_tiles) {
                for (auto [symbol, offset] : tiles_offsets_by_step) {
                    auto current_location = black_tile + offset;
                    if (visited_locations.find(current_location) != visited_locations.end()) continue;
                    visited_locations.insert(current_location);
                    auto black_tiles_neighbors_count = get_black_tiles_neighbors_count(current_location);
                    new_tiles[current_location[0]][current_location[1]] = all_tiles[current_location[0]][current_location[1]] ? black_tiles_neighbors_count > 0 && black_tiles_neighbors_count <= 2 : black_tiles_neighbors_count == 2;
                    if (new_tiles[current_location[0]][current_location[1]]) new_black_tiles.insert(new_black_tiles.end(), current_location);
                }
            }
            std::swap(all_tiles, new_tiles);
            std::swap(black_tiles, new_black_tiles);
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