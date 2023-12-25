#ifndef ADVENTOFCODE2021_RANGE_HPP
#define ADVENTOFCODE2021_RANGE_HPP

#include "point.hpp"

template <size_t Dims>
struct range_xd {
    std::array<point_xd<Dims>, 2> locations;

    bool operator==(const range_xd& ref) const {
        return locations == ref.locations;
    }

    std::pair<int64_t&, int64_t&> operator[](size_t dim) {
        return {locations[0][dim], locations[1][dim]};
    }

    std::pair<int64_t&, int64_t&> operator[](size_t dim) const {
        return {locations[0][dim], locations[1][dim]};
    }

    point_xd<Dims>& start() {
        return locations[0];
    }

    point_xd<Dims> start() const {
        return locations[0];
    }

    point_xd<Dims>& stop() {
        return locations[1];
    }

    point_xd<Dims> stop() const {
        return locations[1];
    }

    auto operator+=(const range_xd& ref) {
        locations[0] += ref[0];
        locations[1] += ref[1];
    }

    auto operator+(const range_xd& ref) const {
        range_xd res;
        res.locations[0] = locations[0] + ref[0];
        res.locations[1] = locations[1] + ref[1];
        return res;
    }

    bool is_intersect_in_dim(const range_xd& ref, size_t dim) const {
        auto ref_start = ref.start()[dim];
        auto ref_stop = ref.stop()[dim];

        auto start_d = start()[dim];
        auto stop_d = stop()[dim];

        return ref_stop >= start_d && ref_start <= stop_d;
    }

    bool is_crossed(const range_xd& ref) const {
        size_t crossed_dims = 0;
        size_t i;
        for (i = 0; i < Dims; ++i) {
            auto ref_start = ref.start()[i];
            auto ref_stop = ref.stop()[i];

            auto start_d = start()[i];
            auto stop_d = stop()[i];

            crossed_dims += ref_stop >= start_d && ref_start <= stop_d;
        }
        return crossed_dims == i;
    }
};

template <>
template <size_t Dims>
struct std::hash<range_xd<Dims>>
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
struct std::hash<range_xd<2>>
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