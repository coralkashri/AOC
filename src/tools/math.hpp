#ifndef ADVENTOFCODE2021_MATH_HPP
#define ADVENTOFCODE2021_MATH_HPP

constexpr auto lcm(const std::vector<size_t>& numbers)
{
    size_t result = numbers[0];
    for (size_t n : numbers) {
        result = std::lcm(result, n);
    }
    return result;
}

#endif //ADVENTOFCODE2021_MATH_HPP
