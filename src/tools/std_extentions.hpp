#ifndef ADVENTOFCODE2021_STD_EXTENTIONS_HPP
#define ADVENTOFCODE2021_STD_EXTENTIONS_HPP

namespace std {

    template<typename InputIterator, typename BinaryOperation, typename Predicate>
    auto accumulate_if(InputIterator first, const InputIterator last, auto init, BinaryOperation &&binary_op,
                       Predicate &&predicate) {
        for (; first != last; ++first)
            if (predicate(*first)) init = binary_op(init, *first);
        return init;
    }
}

#endif //ADVENTOFCODE2021_STD_EXTENTIONS_HPP