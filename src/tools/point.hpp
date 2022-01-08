#ifndef ADVENTOFCODE2021_POINT_HPP
#define ADVENTOFCODE2021_POINT_HPP

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
};

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