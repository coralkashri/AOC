#ifndef ADVENTOFCODE2021_COMPILE_TIME_MATH_HPP
#define ADVENTOFCODE2021_COMPILE_TIME_MATH_HPP

#include "custom_template_conditions.hpp"

namespace compile_time_math {

    template<int Number, bool = custom_conditions::is_bigger_than_v<Number, 0>>
    struct factorial {
        static constexpr long long value = 1;
    };

    template<int Number>
    struct factorial<Number, true> {
        static constexpr auto value = Number * factorial<Number - 1>::value;
    };

    template<int Base, int Power, bool = custom_conditions::is_bigger_than_v<Power, 0>>
    struct pow {
        static constexpr auto value = 1ll;
    };

    template<int Base, int Power>
    struct pow<Base, Power, true> {
        static constexpr auto value = Base * pow<Base, Power - 1>::value;
    };
}

#endif