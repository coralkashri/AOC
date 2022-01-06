#ifndef AOC_2020_8
#define AOC_2020_8

#include "../tools/base_includes.h"

using jmps_nops_collector_t = std::list<size_t>;

auto parse_command(std::string_view command) {
    auto op_code = command.substr(0, 3);
    auto number = command.substr(4);
    return std::pair{op_code, number};
}

void execute_command(std::string &command, size_t &command_line, int &global_value) {
    auto [op_code, number] = parse_command(command);
    auto actual_number = std::stoi(number.begin());
    if (op_code != "jmp") {
        if (op_code == "acc") global_value += actual_number;
        command_line++;
    } else command_line += actual_number;
}

int first_part_2020_8() {
    int global_value = 0;
    std::vector<std::pair<std::string, bool>> commands;
    commands.reserve(1000);
    auto it = std::find_if(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&, current_command_line = 0ul] (std::string str) mutable {
        commands.emplace_back(str, false);
        if (current_command_line >= commands.size()) {
            return false;
        }
        while (current_command_line < commands.size()) {
            if (auto &is_executed = commands.at(current_command_line).second; is_executed) return true;
            else is_executed = true;
            execute_command(commands.at(current_command_line).first, current_command_line, global_value);
        }
        return false;
    });

    std::cout << global_value << std::endl;

    return EXIT_SUCCESS;
}

void execute_command(std::string &command, size_t &command_line, int &global_value, jmps_nops_collector_t &jmps_nops_collector, bool update_jmps_nops_collector) {
    auto [op_code, number] = parse_command(command);
    auto actual_number = std::stoi(number.begin());
    if (op_code != "jmp") {
        if (op_code == "acc") global_value += actual_number;
        else if (update_jmps_nops_collector) jmps_nops_collector.emplace_back(command_line);
        command_line++;
    } else {
        if (update_jmps_nops_collector) jmps_nops_collector.emplace_back(command_line);
        command_line += actual_number;
    }
}

void reset_executed_commands(std::vector<std::pair<std::string, bool>> &commands) {
    for (auto &command : commands) command.second = false;
}

void swap_jmp_nop(std::string &command) {
    auto [op_code, number] = parse_command(command);
    if (op_code == "jmp") command = "nop " + std::string(number);
    else command = "jmp " + std::string(number);
}

int second_part_2020_8() {
    int global_value = 0;
    std::vector<std::pair<std::string, bool>> commands;
    jmps_nops_collector_t visited_jmps_and_nops;
    std::string *last_changed_command = nullptr;
    commands.reserve(1000);
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&, current_command_line = 0ul] (std::string str) mutable {
        commands.emplace_back(str, false);
        if (current_command_line >= commands.size()) return;
        while (current_command_line < commands.size()) {
            if (auto &is_executed = commands.at(current_command_line).second; is_executed) {
                reset_executed_commands(commands);
                if (last_changed_command) swap_jmp_nop(*last_changed_command);
                swap_jmp_nop(commands.at(visited_jmps_and_nops.back()).first);
                last_changed_command = &commands.at(visited_jmps_and_nops.back()).first;
                visited_jmps_and_nops.pop_back();
                current_command_line = 0;
                global_value = 0;
            } else {
                is_executed = true;
                execute_command(commands.at(current_command_line).first, current_command_line, global_value, visited_jmps_and_nops, !last_changed_command);
            }
        }
    });

    std::cout << global_value << std::endl;

    return EXIT_SUCCESS;
}

#endif