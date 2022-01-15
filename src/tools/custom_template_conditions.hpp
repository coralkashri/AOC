/* Taken from: https://github.com/coralkashri/CppTemplateCustomConditions */

#ifndef CPP_TEMPLATE_CUSTOM_CONDITIONS_CUSTOM_TEMPLATE_CONDITIONS_HPP
#define CPP_TEMPLATE_CUSTOM_CONDITIONS_CUSTOM_TEMPLATE_CONDITIONS_HPP

#include <type_traits>

namespace custom_conditions {

    // general_compare

    template<int Fst, int Snd, typename Op, typename = void>
    struct general_compare : std::false_type {};

    template<int Fst, int Snd, typename Op>
    struct general_compare<Fst, Snd, Op, typename std::enable_if_t<(Op()(Fst, Snd)), void>> : std::true_type {};

    template<int Fst, int Snd, typename Op>
    constexpr bool general_compare_v = general_compare<Fst, Snd, Op>::value;

    // is_bigger_than

    template<int Fst, int Snd>
    constexpr bool is_bigger_than_v = general_compare_v<Fst, Snd, std::greater<>>;

    template<int Fst, int Snd>
    constexpr bool is_bigger_equal_v = general_compare_v<Fst, Snd, std::greater_equal<>>;

    template<int Fst, int Snd>
    constexpr bool is_lower_than_v = general_compare_v<Fst, Snd, std::less<>>;

    template<int Fst, int Snd>
    constexpr bool is_lower_equal_v = general_compare_v<Fst, Snd, std::less_equal<>>;

    template<int Fst, int Snd>
    constexpr bool is_equal_to_v = general_compare_v<Fst, Snd, std::equal_to<>>;

    template<int Fst, int Snd>
    constexpr bool is_not_equal_to_v = general_compare_v<Fst, Snd, std::not_equal_to<>>;
}

#endif