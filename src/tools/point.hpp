#ifndef ADVENTOFCODE2021_POINT_HPP
#define ADVENTOFCODE2021_POINT_HPP

#include <span>
#include "hash_tools.hpp"

template <size_t Dims>
struct point_xd {
    std::array<int64_t, Dims> places;

    bool operator<(const point_xd<Dims> &ref) const {
        return places < ref.places;
    }

    bool operator==(const point_xd& ref) const {
        return places == ref.places;
    }

    auto& operator[](size_t dim) {
        return places[dim];
    }

    auto operator[](size_t dim) const {
        return places[dim];
    }

    auto operator+=(const point_xd& ref) {
        std::transform(places.begin(), places.end(), ref.places.begin(), places.begin(), std::plus<>{});
    }

    auto operator+(const point_xd& ref) const {
        point_xd res;
        std::transform(places.begin(), places.end(), ref.places.begin(), res.places.begin(), std::plus<>{});
        return res;
    }

    auto operator-=(const point_xd& ref) {
        std::transform(places.begin(), places.end(), ref.places.begin(), places.begin(), std::minus<>{});
    }

    auto operator-(const point_xd& ref) const {
        point_xd res;
        std::transform(places.begin(), places.end(), ref.places.begin(), res.places.begin(), std::minus<>{});
        return res;
    }

    auto operator*(long long n) const {
        point_xd res;
        std::transform(places.begin(), places.end(), res.places.begin(), [n](long long p) {
            return p * n;
        });
        return res;
    }

    [[nodiscard]] size_t simple_distance(const point_xd other) const {
        size_t distance = 0;
        for (size_t dim = 0; dim < Dims; ++dim) {
            distance += std::abs(places[dim] - other.places[dim]);
        }
        return distance;
    }
};

namespace point_tools {

    enum direction_names {
        up,
        right_up,
        right,
        right_down,
        down,
        left_down,
        left,
        left_up,
    };

    enum direction_names_advanced {
        north,
        north_east,
        east,
        south_east,
        south,
        south_west,
        west,
        north_west,
    };

    template <typename DirectionsT, bool UseBasicDirections, bool Circular = false>
    class directions_iterator {
    public:
        directions_iterator() : value(0) {}

        directions_iterator(DirectionsT start) : value(start) {
            if constexpr (UseBasicDirections) {
                if (value % 2 == 1) throw std::runtime_error("directions_iterator initialized with UseBasicDirections but got non basic initial direction value.");
            }
        }

        DirectionsT operator*() { return static_cast<DirectionsT>(value); }

        directions_iterator begin() { return directions_iterator<DirectionsT, UseBasicDirections>(static_cast<DirectionsT>(0)); }
        directions_iterator end() { return directions_iterator<DirectionsT, UseBasicDirections>(static_cast<DirectionsT>(8)); }

        bool operator==(const directions_iterator& ref) const {
            return value == ref.value;
        }

        directions_iterator& operator++() {
            if constexpr (Circular) {
                value = (value + 1 + UseBasicDirections) % (north_west + 1);
            } else {
                value += 1 + UseBasicDirections;
            }
            return *this;
        }

        directions_iterator operator++(int) {
            auto ret = *this;
            if constexpr (Circular) {
                value = (value + 1 + UseBasicDirections) % (north_west + 1);
            } else {
                value += 1 + UseBasicDirections;
            }
            return ret;
        }

        directions_iterator& operator--() {
            --value;
            if constexpr (Circular) {
                if (value < 0) value += north_west + 1;
            }
            return *this;
        }

        directions_iterator operator--(int) {
            auto ret = *this;
            --value;
            if constexpr (Circular) {
                if (value < 0) value += north_west + 1;
            }
            return ret;
        }

    private:
        std::underlying_type_t<direction_names_advanced> value;
    };

    class directions {
    public:
        using data_t = std::vector<point_xd<2>>;

        directions() : all_directions({
                                              {0,  -1},
                                              {1,  -1},
                                              {1,  0},
                                              {1,  1},
                                              {0,  1},
                                              {-1, 1},
                                              {-1, 0},
                                              {-1, -1}
                                      }) {
        }

        data_t get_all_directions(std::span<direction_names> order) {
            if (order.empty()) {
                return all_directions;
            }

            std::vector<point_xd<2>> res;
            for (auto d: order) {
                res.push_back(all_directions[d]);
            }
            return res;
        }

        data_t get_all_directions_advanced(std::span<direction_names_advanced> order) {
            if (order.empty()) {
                return all_directions;
            }

            std::vector<point_xd<2>> res;
            for (auto d: order) {
                res.push_back(all_directions[d]);
            }
            return res;
        }

        data_t get_basic_directions(std::span<direction_names> order) {
            if (order.empty()) {
                return {all_directions[up], all_directions[right], all_directions[down], all_directions[left]};
            }

            std::vector<point_xd<2>> res;
            for (auto d: order) {
                res.push_back(all_directions[d]);
            }
            return res;
        }

        data_t get_basic_directions_advanced(std::span<direction_names_advanced> order) {
            if (order.empty()) {
                return {all_directions[north], all_directions[east], all_directions[south], all_directions[west]};
            }

            std::vector<point_xd<2>> res;
            for (auto d: order) {
                res.push_back(all_directions[d]);
            }
            return res;
        }

        template <typename DirectionT>
        requires std::is_same_v<DirectionT, direction_names> || std::is_same_v<DirectionT, direction_names_advanced>
        point_xd<2> get_direction(DirectionT d) {
            return all_directions[d];
        }

    private:
        const data_t all_directions;
    };
}

template <>
template <size_t Dims>
struct std::hash<point_xd<Dims>>
{
    std::size_t operator()(const point_xd<Dims>& point) const
    {
        std::size_t seed = 0;
        for (size_t i = 0; i < Dims; i++) {
            hash_combine(seed, point[i]);
        }
        return seed;
    }
};

template <>
template <>
struct std::hash<point_xd<2>>
{
    std::size_t operator()(const point_xd<2>& point) const
    {
        std::size_t seed = 0;
        hash_combine(seed, point[0]);
        hash_combine(seed, point[1]);
        return seed;
    }
};

#endif