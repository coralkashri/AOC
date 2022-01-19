#ifndef ADVENTOFCODE2021_EFFICIENT_STACK_HPP
#define ADVENTOFCODE2021_EFFICIENT_STACK_HPP

template <typename T, size_t Size>
class efficient_stack {
public:
    void push(const T &elem) {
        cont[current_place++] = std::move(elem);
    }

    T& pop() {
        return cont[--current_place];
    }

    T& top() {
        return cont[current_place - 1];
    }

    bool empty() const {
        return current_place == 0;
    }

    size_t size() const {
        return current_place;
    }

    void clear() {
        current_place = 0;
    }

private:
    std::array<T, Size> cont;
    size_t current_place = 0;
};

#endif