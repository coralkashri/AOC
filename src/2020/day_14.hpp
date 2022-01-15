#ifndef AOC_2020_14
#define AOC_2020_14

#include "../tools/base_includes.h"

template <typename T>
constexpr uint8_t bits_count = sizeof(T) * 8;

template <size_t Version>
class memory_management {
private:
    using mask_t = std::bitset<36>;

public:
    void set_mask(std::string_view mask) {
        if constexpr (Version == 1) set_mask_ver1(mask);
        else {
            floating_masks.clear();
            floating_mask_points.clear();
            set_mask_ver2(mask, mask_t{});
        }
    }

    void set_value(uint64_t addr, uint64_t value) {
        if constexpr (Version == 1) {
            current_sum -= memory[addr];
            memory[addr] = apply_mask(value);
            current_sum += memory[addr];
        } else {
            auto possible_addrs = apply_memory_floating_masks(addr);
            for (const auto &possible_addr : possible_addrs) {
                current_sum -= memory[possible_addr.to_ullong()];
                memory[possible_addr.to_ullong()] = value;
                current_sum += memory[possible_addr.to_ullong()];
            }
        }
    }

    [[nodiscard]] uint64_t get_sum() const {
        return current_sum;
    }

private:
    mask_t zero_mask;
    mask_t one_mask;
    std::vector<mask_t> floating_masks;
    std::set<uint8_t> floating_mask_points;
    std::unordered_map<uint64_t, uint64_t> memory;
    uint64_t current_sum = 0;

    [[nodiscard]] uint64_t apply_mask(const mask_t &value) const {
        return ((value & zero_mask) | one_mask).to_ullong();
    }

    [[nodiscard]] auto apply_memory_floating_masks(const mask_t &addr) const {
        std::vector<mask_t> possible_addresses;
        possible_addresses.reserve(floating_masks.size());
        for (auto &mask : floating_masks) {
            possible_addresses.template emplace_back((addr | mask).to_ullong());
            for (auto &point : floating_mask_points) {
                possible_addresses.back()[point] = mask[point];
            }
        }
        return possible_addresses;
    }

    void set_mask_ver1(std::string_view mask) {
        zero_mask = -1;
        one_mask = 0;
        for (size_t i = 0; i < mask.size(); i++) {
            if (mask[i] == 'X') continue;
            if (mask[i] == '1') one_mask |= 1ull << (mask.size() - 1 - i);
            else zero_mask &= ~(1ull << (mask.size() - 1 - i));
        }
    }

    void set_mask_ver2(std::string_view mask, mask_t modified_mask, size_t idx = 0) {
        for (size_t i = idx; i < mask.size(); i++) {
            if (mask[i] == '0') continue;
            if (mask[i] == 'X') {
                mask_t possible_mask_1 = modified_mask;
                floating_mask_points.insert(mask.size() - 1 - i);
                set_mask_ver2(mask, possible_mask_1 |= (1ull << (mask.size() - 1 - i)), i + 1);
                set_mask_ver2(mask, modified_mask, i + 1);
                return;
            } else if (mask[i] == '1') modified_mask |= 1ull << (mask.size() - 1 - i);
        }
        floating_masks.template emplace_back(modified_mask);
    }
};

template <size_t Version>
void update_memory(memory_management<Version> &mem) {
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (std::string str) mutable {
        if (str.find("mask") != std::string::npos) {
            mem.set_mask(str.substr(str.find('=') + 2));
            return;
        }
        auto open_mem_addr = str.find('[');
        auto close_mem_addr = str.find(']');
        auto addr = std::stoll(str.substr(open_mem_addr + 1, close_mem_addr - open_mem_addr - 1));
        auto value = std::stoll(str.substr(close_mem_addr + 4));
        mem.set_value(addr, value);
    });
}

int first_part_2020_14() {
    memory_management<1> mem;

    update_memory(mem);

    std::cout << mem.get_sum() << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2020_14() {
    memory_management<2> mem;

    update_memory(mem);

    std::cout << mem.get_sum() << std::endl;

    return EXIT_SUCCESS;
}

#endif