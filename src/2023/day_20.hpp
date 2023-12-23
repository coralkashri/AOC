#ifndef AOC_2023_20
#define AOC_2023_20

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include <array>
#include <set>
#include <queue>
#include "../input_source.hpp"
#include "../tools/base_includes.h"

enum pulse_types {
    High,
    Low
};

class module {
public:
    std::string name;

    void register_destination(const std::string& dest) {
        destination_names.push_back(dest);
    }

    void activate_destinations(auto& all_modules) {
        for (const auto& dest_name : destination_names) {
            if (all_modules.contains(dest_name)) {
                auto &dest = all_modules.at(dest_name);
                destinations.push_back(dest);
                dest->register_input_source(name);
            } else {
                destinations.push_back(nullptr);
            }
        }
    }

    virtual void reset() {}

    virtual void register_pulse(const std::string& src, pulse_types t) {};

    virtual void register_input_source(const std::string& source_name) {}

    virtual std::pair<std::vector<std::shared_ptr<module>>, pulse_types> generate_pulse() {
        return generate_pulse_impl(Low);
    }

protected:
    std::pair<std::vector<std::shared_ptr<module>>, pulse_types> generate_pulse_impl(pulse_types t) {
        for (auto &c : destinations) {
            if (c) c->register_pulse(name, t);
        }

        return {destinations, t};
    }

private:
    std::vector<std::shared_ptr<module>> destinations;
    std::vector<std::string> destination_names;
};

class flip_flop_module : public module {
public:
    void reset() override {
        is_on = false;
        last_received_pulse_types.clear();
    }

    void register_pulse(const std::string& src, pulse_types t) override {
        if (t == Low) last_received_pulse_types.push_back(t);
    }

    std::pair<std::vector<std::shared_ptr<module>>, pulse_types> generate_pulse() override {
        std::pair<std::vector<std::shared_ptr<module>>, pulse_types> res;

        for (auto p : last_received_pulse_types) {
            is_on = !is_on;

            res = generate_pulse_impl(is_on ? High : Low);
        }
        last_received_pulse_types.clear();
        return res;
    }

private:
    bool is_on = false;
    std::vector<pulse_types> last_received_pulse_types;
};

class conjunction_module : public module {
public:
    void reset() override {
        for (auto& p : previous_pulses) {
            p.second = Low;
        }
    }

    void register_pulse(const std::string& src, pulse_types t) override {
        previous_pulses[src] = t;
    }

    void register_input_source(const std::string &source_name) override {
        previous_pulses[source_name] = Low;
    }

    std::pair<std::vector<std::shared_ptr<module>>, pulse_types> generate_pulse() override {
        bool is_all_high = true;
        for (auto& p : previous_pulses) {
            if (p.second == Low) {
                is_all_high = false;
                break;
            }
        }

        return generate_pulse_impl(is_all_high ? Low : High);
    }

private:
    std::unordered_map<std::string, pulse_types> previous_pulses;
};

std::unique_ptr<module> create_module(char type) {
    std::unique_ptr<module> m;
    if (type == '%') {
        m = std::make_unique<flip_flop_module>();
    } else if (type == '&') {
        m = std::make_unique<conjunction_module>();
    } else {
        m = std::make_unique<module>();
    }
    return m;
}

auto parse_modules() {
    std::unordered_map<std::string, std::shared_ptr<module>> all_modules;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE),
                  std::istream_iterator<WordDelimitedBy<'\n'>>(),
                  [&all_modules](std::string str) {
                      std::string_view str_view(str);

                      std::shared_ptr<module> m = create_module(str_view[0]);

                      auto module_name_end = str_view.find(' ');
                      m->name = str_view.substr(str_view[0] != 'b', module_name_end - (str_view[0] != 'b'));

                      str_view = str_view.substr(module_name_end + 4);

                      auto next_dest_module_index = str_view.find(',');

                      std::string dest_module_name;
                      while (next_dest_module_index != std::string::npos) {
                          dest_module_name = str_view.substr(0, next_dest_module_index);
                          m->register_destination(dest_module_name);

                          str_view = str_view.substr(next_dest_module_index + 2);
                          next_dest_module_index = str_view.find(',');
                      }
                      dest_module_name = str_view.substr(0, next_dest_module_index);
                      m->register_destination(dest_module_name);

                      all_modules[m->name] = m;
                  });
    return all_modules;
}

void activate_modules(auto& all_modules) {
    for (auto& m : all_modules) {
        m.second->activate_destinations(all_modules);
    }
}

int first_part_2023_20() {
    std::unordered_map<std::string, std::shared_ptr<module>> all_modules;
    std::queue<std::shared_ptr<module>> next_modules_to_activate;
    size_t low_pulses = 0, high_pulses = 0;
    all_modules = parse_modules();
    activate_modules(all_modules);

    for (size_t i = 0; i < 1000; ++i) {
        ++low_pulses;
        next_modules_to_activate.push(all_modules["broadcaster"]);

        while (!next_modules_to_activate.empty()) {
            auto src = next_modules_to_activate.front();
            next_modules_to_activate.pop();

            if (!src) continue;

            auto res = src->generate_pulse();
            low_pulses += res.first.size() * (res.second == Low);
            high_pulses += res.first.size() * (res.second == High);

            for (auto &c: res.first) {
                next_modules_to_activate.push(c);
            }
        }
    }

    std::cout << low_pulses * high_pulses << std::endl;

    return EXIT_SUCCESS;
}

size_t count_button_clicks_for_high(const auto& all_modules, std::string_view module_name) {
    std::queue<std::shared_ptr<module>> next_modules_to_activate;
    size_t i;
    for (i = 0; i < -1;) {
        ++i;
        next_modules_to_activate.push(all_modules.at("broadcaster"));

        while (!next_modules_to_activate.empty()) {
            auto src = next_modules_to_activate.front();
            next_modules_to_activate.pop();

            if (!src) continue;

            auto res = src->generate_pulse();
            for (auto &c: res.first) {
                next_modules_to_activate.push(c);
            }

            if (src->name == module_name && res.second == High) {
                return i;
            }
        }
    }
    return -1;
}

void reset_all_modules(auto& modules) {
    for (auto& m : modules) {
        (*m.second).reset();
    }
}

int second_part_2023_20() {
    std::unordered_map<std::string, std::shared_ptr<module>> all_modules;
    std::queue<std::shared_ptr<module>> next_modules_to_activate;
    all_modules = parse_modules();
    activate_modules(all_modules);

    size_t xj = count_button_clicks_for_high(all_modules, "xj");
    reset_all_modules(all_modules);
    size_t qs = count_button_clicks_for_high(all_modules, "qs");
    reset_all_modules(all_modules);
    size_t kz = count_button_clicks_for_high(all_modules, "kz");
    reset_all_modules(all_modules);
    size_t km = count_button_clicks_for_high(all_modules, "km");

    std::cout << lcm({xj, qs, kz, km}) << std::endl;

    return EXIT_SUCCESS;
}

#endif
