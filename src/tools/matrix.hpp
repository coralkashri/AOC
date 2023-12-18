#ifndef ADVENTOFCODE2021_MATRIX_HPP
#define ADVENTOFCODE2021_MATRIX_HPP

namespace aoc_tools {

    template<typename T>
    class matrix {
    public:
        void insert_line(const std::vector<T> &str) {
            set_width_if_not_set(str.size());
            data.insert(data.end(), str.begin(), str.end());
        }

        void insert_line(std::string_view str) {
            set_width_if_not_set(str.size());
            data.insert(data.end(), str.begin(), str.end());
        }

        void insert_line_front(const std::vector<T> &str) {
            set_width_if_not_set(str.size());
            data.insert(data.begin(), str.begin(), str.end());
        }

        void insert_line_front(std::string_view str) {
            set_width_if_not_set(str.size());
            data.insert(data.begin(), str.begin(), str.end());
        }

        [[nodiscard]] T operator[](size_t x, size_t y) const {
            return data[y * width + x];
        }

        [[nodiscard]] T &operator[](size_t x, size_t y) {
            return data[y * width + x];
        }

        [[nodiscard]] size_t size_x() const {
            return width;
        }

        [[nodiscard]] size_t size_y() const {
            return data.size() / width;
        }

        void print() {
            for (size_t y = 0; y < size_y(); ++y) {
                for (size_t x = 0; x < size_x(); ++x) {
                    char c = (*this)[x, y];
                    std::cout << (c == '.' ? ' ' : c);
                }
                std::cout << "\n";
            }
        }

    private:
        std::vector<T> data;
        size_t width = 0;

        void set_width_if_not_set(size_t new_width) {
            if (!width) width = new_width;
        }
    };
}

#endif
