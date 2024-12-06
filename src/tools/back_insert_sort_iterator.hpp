#ifndef ADVENTOFCODE2021_BACK_INSERT_SORT_ITERATOR_HPP
#define ADVENTOFCODE2021_BACK_INSERT_SORT_ITERATOR_HPP

#include <iterator>

template<typename ContT, typename CompT = std::less<typename ContT::value_type>>
class back_insert_sort_iterator : private std::back_insert_iterator<ContT> {
public:
    explicit back_insert_sort_iterator(ContT &cont) : std::back_insert_iterator<ContT>(cont) {}

    back_insert_sort_iterator &operator=(const typename ContT::value_type &value) {
        container->insert(std::lower_bound(container->begin(), container->end(), value, CompT{}), value);
        return *this;
    }

    back_insert_sort_iterator &operator*() { return *this; }

    back_insert_sort_iterator &operator++() { return *this; }

    back_insert_sort_iterator &operator++(int) { return *this; }

private:
    using std::back_insert_iterator<ContT>::container;
};

#endif //ADVENTOFCODE2021_BACK_INSERT_SORT_ITERATOR_HPP