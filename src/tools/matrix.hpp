#ifndef ADVENTOFCODE2021_MATRIX_HPP
#define ADVENTOFCODE2021_MATRIX_HPP

namespace aoc_tools {

    template<typename T>
    class matrix {
    public:
        void set_width_and_height(size_t new_width, size_t new_height, T default_value = T()) {
            width = new_width;
            data.resize(new_width * new_height, default_value);
        }

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

        template <typename Conversion = T>
        void print(T space_char = -1) {
            for (size_t y = 0; y < size_y(); ++y) {
                for (size_t x = 0; x < size_x(); ++x) {
                    char c = (*this)[x, y];
                    if (c == space_char)
                        std::cout << " ";
                    else std::cout << (Conversion)c;
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
