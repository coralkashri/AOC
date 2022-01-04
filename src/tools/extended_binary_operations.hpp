#ifndef AOC_EXTENDED_BINARY_OPERATIONS_H
#define AOC_EXTENDED_BINARY_OPERATIONS_H

template<typename Tp = void>
struct max;

template<typename Tp>
struct max : public std::binary_function<Tp, Tp, Tp>
{
    constexpr Tp operator()(const Tp& x, const Tp& y) const { return std::max(x, y); }
};

template<>
struct max<void>
{
    template <typename Tp, typename Up>
    constexpr auto operator()(Tp&& t, Up&& u) const noexcept(noexcept(std::max(std::forward<Tp>(t), std::forward<Up>(u))))
    -> decltype(std::max(std::forward<Tp>(t), std::forward<Up>(u))) { return std::max(std::forward<Tp>(t), std::forward<Up>(u)); }

    typedef std::__is_transparent is_transparent;
};

template<typename Tp = void>
struct min;

template<typename Tp>
struct min : public std::binary_function<Tp, Tp, Tp>
{
    constexpr Tp operator()(const Tp& x, const Tp& y) const { return std::min(x, y); }
};

template<>
struct min<void>
{
    template <typename Tp, typename Up>
    constexpr auto operator()(Tp&& t, Up&& u) const noexcept(noexcept(std::min(std::forward<Tp>(t), std::forward<Up>(u))))
    -> decltype(std::min(std::forward<Tp>(t), std::forward<Up>(u))) { return std::min(std::forward<Tp>(t), std::forward<Up>(u)); }

    typedef std::__is_transparent is_transparent;
};

#endif