#ifndef ADVENTOFCODE2021_HASH_TOOLS_HPP
#define ADVENTOFCODE2021_HASH_TOOLS_HPP

template <class T>
inline void hash_combine(std::size_t & seed, const T & v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <>
template <typename T>
struct std::hash<std::queue<T>>
{
std::size_t operator()(queue<T> q) const
{
    std::size_t seed = 0;
    while (!q.empty()) {
        hash_combine(seed, q.front());
        q.pop();
    }
    return seed;
}
};

namespace hash_tuple{

    template <typename TT>
    struct hash
    {
        size_t
        operator()(TT const& tt) const
        {
            return std::hash<TT>()(tt);
        }
    };
}

namespace hash_tuple{

    namespace
    {
        // Recursive template code derived from Matthieu M.
        template <class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
        struct HashValueImpl
        {
            static void apply(size_t& seed, Tuple const& tuple)
            {
                HashValueImpl<Tuple, Index-1>::apply(seed, tuple);
                hash_combine(seed, std::get<Index>(tuple));
            }
        };

        template <class Tuple>
        struct HashValueImpl<Tuple,0>
        {
            static void apply(size_t& seed, Tuple const& tuple)
            {
                hash_combine(seed, std::get<0>(tuple));
            }
        };
    }

    template <typename ... TT>
    struct hash<std::tuple<TT...>>
    {
        size_t
        operator()(std::tuple<TT...> const& tt) const
        {
            size_t seed = 0;
            HashValueImpl<std::tuple<TT...> >::apply(seed, tt);
            return seed;
        }
    };

}

#endif