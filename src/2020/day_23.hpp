#ifndef AOC_2020_23
#define AOC_2020_23

#include "../tools/base_includes.h"

namespace aoc_2020_23_tools {
    template <bool IsAdvancedTranslator>
    class crab_translator {
    public:
        crab_translator() {
            read_base_key();
            initialize_translator_numbers();
            initialize_quick_list_access();
        }

        void translate() {
            if constexpr(IsAdvancedTranslator) {
                advanced_translator();
            } else {
                simple_translator();
            }
        }

    private:
        std::string base_key;
        int max_number;
        std::list<int> numbers;
        std::unordered_map<int, typename decltype(numbers)::iterator> quick_list_access;

        template <typename T>
        void print_container(const T& cont) {
            std::copy(cont.begin(), cont.end(), std::ostream_iterator<int>(std::cout));
            std::cout << "\n";
        }

        template <typename T>
        void formatted_numbers_print(const T& cont, typename T::iterator current_number_it) {
            for (auto it = cont.begin(); it != cont.end(); ++it) {
                if (it == current_number_it) std::cout << "( " << *it << " ) ";
                else std::cout << *it << " ";
            }
            std::cout << "\n";
        }

        template <bool PrintOnlySampleAfterOne = false>
        void print_numbers_aligned_to_1() {
            auto it_1 = std::find(numbers.begin(), numbers.end(), 1);

            if constexpr(PrintOnlySampleAfterOne) {
                std::copy(it_1, std::next(it_1, 10), std::ostream_iterator<int>(std::cout, ", "));
            } else {
                std::copy(it_1, numbers.end(), std::ostream_iterator<int>(std::cout, ", "));
                std::copy(numbers.begin(), it_1, std::ostream_iterator<int>(std::cout, ", "));
            }

            std::cout << "\n";
        };

        decltype(numbers)::iterator update_current_number_it(decltype(numbers)::iterator current_number_it) {
            ++current_number_it;
            if (current_number_it == numbers.end()) current_number_it = numbers.begin();
            return current_number_it;
        };

        template <int RemoveCount = 3>
        void remove_numbers(decltype(numbers)::iterator start_it, std::vector<int> &removed_numbers) {
            for (size_t i = 0; i < RemoveCount; ++i) {
                if (start_it == numbers.end()) start_it = numbers.begin();
                removed_numbers[i] = *start_it;
                quick_list_access[*start_it] = numbers.end();
                start_it = numbers.erase(start_it);
            }
        };

        template <int RemoveCount = 3>
        void place_numbers(decltype(numbers)::iterator place_it, const std::vector<int> &removed_numbers) {
            auto it = numbers.insert(place_it, removed_numbers.begin(),  removed_numbers.end());

            int counter = 0;
            for (; counter++ < RemoveCount + 2; it = std::next(it)) {
                if (it == numbers.end()) it = numbers.begin();
                quick_list_access[*it] = it;
            }
        };

        bool is_in_removed_numbers(int number, const std::vector<int> &removed_numbers) const {
            return std::find(removed_numbers.begin(),  removed_numbers.end(), number) != removed_numbers.end();
        };

        decltype(numbers)::iterator find_next_insert_location(decltype(numbers)::iterator current_number_it, const std::vector<int> &removed_numbers) {
            int target_number = *current_number_it;
            do {
                --target_number;
                if (target_number == 0) target_number = max_number;
            } while (is_in_removed_numbers(target_number, removed_numbers));

            return quick_list_access[target_number];
        }

        template <bool Verbose>
        void translator_loop(size_t iterations_count) {
            std::vector<int> removed_numbers(3);
            auto current_number_it = numbers.begin();

            for (int i = 0; i < iterations_count; i++) {
                if constexpr(Verbose) {
                    std::cout << "Move: " << i + 1 << "\n";

                    print_container(numbers);
                }

                remove_numbers(std::next(current_number_it), removed_numbers);

                if constexpr(Verbose) {
                    print_container(removed_numbers);
                }

                auto it = find_next_insert_location(current_number_it, removed_numbers);

                if constexpr(Verbose) {
                    std::cout << "Target location: " << *it << "\n";
                }

                place_numbers(std::next(it), removed_numbers);
                current_number_it = update_current_number_it(current_number_it);
            }
        }

        void simple_translator() {
            size_t iterations_count = 100;

            translator_loop<false>(iterations_count);

            std::cout << "Result: ";
            print_container(numbers);
            std::cout << "Result (aligned to 1): ";
            print_numbers_aligned_to_1();
        }

        void advanced_translator() {
            size_t iterations_count = 10e6;

            translator_loop<false>(iterations_count);

            std::cout << "Result: ";
            print_numbers_aligned_to_1<true>();

            std::cout << "Result (multiply): ";
            auto it = std::find(numbers.begin(), numbers.end(), 1);
            std::cout << (uint64_t)*std::next(it) * (uint64_t)*std::next(it, 2) << "\n";
        }

        void read_base_key() {
            INPUT_SOURCE >> base_key;
        }

        void initialize_translator_numbers() {
            if constexpr (IsAdvancedTranslator) {
                numbers = decltype(numbers)(1e6);
                std::iota(std::next(numbers.begin(), 9), numbers.end(), 10);
            } else {
                numbers = decltype(numbers)(9);
            }
            max_number = numbers.size();
            std::transform(base_key.begin(), base_key.end(), numbers.begin(), [](unsigned char c) { return c - '0'; });
        }

        void initialize_quick_list_access() {
            for (auto it = numbers.begin(); it != numbers.end(); ++it) {
                quick_list_access[*it] = it;
            }
        }
    };
}

int first_part_2020_23() {
    aoc_2020_23_tools::crab_translator<false> translator;

    translator.translate();

    return EXIT_SUCCESS;
}

int second_part_2020_23() {
    aoc_2020_23_tools::crab_translator<true> translator;

    translator.translate();

    return EXIT_SUCCESS;
}

#endif