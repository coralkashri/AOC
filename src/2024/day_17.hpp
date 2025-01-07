#ifndef ADVENTOFCODE2024_DAY_17_HPP
#define ADVENTOFCODE2024_DAY_17_HPP

#include <iostream>
#include <algorithm>
#include <iterator>
#include <map>
#include <ranges>
#include <numeric>
#include <regex>
#include <cmath>

#include "../input_source.hpp"
#include "../tools/str_number.hpp"
#include "../tools/word_delimited_by.h"
#include "../tools/matrix.hpp"
#include "../tools/point.hpp"
#include <set>
#include <thread>
#include "../tools/range.hpp"

#include "../tools/linear_algebra/all.hpp"

struct registers {
    static inline size_t A = 0;
    static inline size_t B = 0;
    static inline size_t C = 0;
    static inline long long IP = 0;

    enum OperandType {
        LiteralOperand,
        ComboOperand
    };

    static size_t get_operand(int operand, OperandType type) {
        switch (type) {
            case LiteralOperand:
                return operand;

            case ComboOperand:
                switch (operand) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                        return operand;
                    case 4:
                        return A;
                    case 5:
                        return B;
                    case 6:
                        return C;
                    case 7:
                    default:
                        exit(1);
                }
        }
        exit(1);
    }
};

class op_code {
public:
    virtual ~op_code() = default;

    virtual void apply(int n, std::ostream &output) = 0;

    virtual void reverse_apply(int n) = 0;
};

class adv : public op_code { // OpCode: 0
public:
    void apply(int n, std::ostream &output) override {
        registers::A = registers::A / std::pow(2, registers::get_operand(n, registers::ComboOperand)); // A = A / 8 == A >> 3
        registers::IP += 2;
    }

    void reverse_apply(int n) override {
        registers::A = registers::A * std::pow(2, registers::get_operand(n, registers::ComboOperand)); // A = A / 8 == A >> 3
        registers::IP -= 2;
    }
};

class bxl : public op_code { // OpCode: 1
public:
    void apply(int n, std::ostream &output) override {
        registers::B = registers::get_operand(n, registers::LiteralOperand) ^ registers::B; // B = Take 3 (lsb) bits from B and change them
        registers::IP += 2;
    }

    void reverse_apply(int n) override {
        registers::B = registers::get_operand(n, registers::LiteralOperand) ^ registers::B; // B = Take 3 (lsb) bits from B and change them
        registers::IP -= 2;
    }
};

class bst : public op_code { // OpCode: 2
public:
    void apply(int n, std::ostream &output) override {
        registers::B = registers::get_operand(n, registers::ComboOperand) % 8; // B = A % 8 == Take 3 bits from A
        registers::IP += 2;
    }

    void reverse_apply(int n) override {
        registers::A <<= 3; // B = A % 8 == Take 3 bits from A
        registers::A |= registers::B % 8;
        registers::IP -= 2;
    }
};

class jnz : public op_code { // OpCode: 3
public:
    void apply(int n, std::ostream &output) override {
        if (registers::A != 0) {
            registers::IP = registers::get_operand(n, registers::LiteralOperand);
        } else {
            registers::IP += 2;
        }
    }

    void reverse_apply(int n) override {
        registers::IP -= 2;
    }
};

class bxc : public op_code { // OpCode: 4
public:
    void apply(int n, std::ostream &output) override {
        registers::B = registers::B ^ registers::C; // B = B ^ C
        registers::IP += 2;
    }

    void reverse_apply(int n) override {
        registers::B = registers::B ^ registers::C; // B = B ^ C
        registers::IP -= 2;
    }
};

class out : public op_code { // OpCode: 5
public:
    void apply(int n, std::ostream &output) override {
        output << registers::get_operand(n, registers::ComboOperand) % 8 << ","; // Print B % 8
        registers::IP += 2;
    }

    void reverse_apply(int n) override {
        std::cout << registers::A << ","; // Print B % 8
        registers::IP -= 2;
    }
};

class bdv : public op_code { // OpCode: 6
public:
    void apply(int n, std::ostream &output) override {
        registers::B = registers::A / std::pow(2, registers::get_operand(n, registers::ComboOperand));
        registers::IP += 2;
    }

    void reverse_apply(int n) override {

    }
};

class cdv : public op_code { // OpCode: 7
public:
    void apply(int n, std::ostream &output) override {
        registers::C = registers::A / std::pow(2, registers::get_operand(n, registers::ComboOperand)); // C = A / 2^B == B- 0-7 -> A/1-128
        registers::IP += 2;
    }

    void reverse_apply(int n) override {
        registers::A <<= registers::B; // C = A / 2^B == B- 0-7 -> A/1-128
        registers::IP -= 2;
    }
};

class program {
public:
    program() : all_op_codes({
                                     std::make_unique<adv>(),
                                     std::make_unique<bxl>(),
                                     std::make_unique<bst>(),
                                     std::make_unique<jnz>(),
                                     std::make_unique<bxc>(),
                                     std::make_unique<out>(),
                                     std::make_unique<bdv>(),
                                     std::make_unique<cdv>()
                             }) {}

    void parse_register(std::string_view str) {
        auto val = str_number<size_t>(str.substr(12)).get_number();
        switch (str[9]) {
            case 'A':
                registers::A = val;
                break;
            case 'B':
                registers::B = val;
                break;
            case 'C':
                registers::C = val;
                break;
            default:
                exit(1);
        }
    }

    void parse_command(std::string_view str) {
        command_str = str.substr(9);
        commands = str_numbers<int, std::vector, ','>(command_str).get();
        command_str += ',';
    }

    void run(std::ostream &output) {
        while (registers::IP < commands.size()) {
            all_op_codes[commands[registers::IP]]->apply(commands[registers::IP + 1], output);
            registers::IP = registers::IP; // suppress a warning
        }
    }

    size_t find_initial_a_register_to_print_the_program() {
        bool found = false;
        registers::A = find_initial_a_register_to_print_the_program(commands, 0, found);
        return registers::A;
    }

    void print_command() {
        std::cout << command_str << "\n";
    }

    std::string get_command() {
        return command_str;
    }

private:
    std::array<std::unique_ptr<op_code>, 8> all_op_codes;
    std::vector<int> commands;
    std::string command_str;

    size_t find_initial_a_register_to_print_the_program(std::span<int> next_commands, uint64_t prev_a, bool &found) {
        if (next_commands.empty()) {
            found = true;
            return prev_a;
        }

        std::stringstream output;
        uint64_t prev_b = 0;
        while (prev_b < 8 && !found && (output.str().empty() || output.str()[0] != '0' + next_commands.back())) {
            output.str("");
            registers::A = prev_a * 8;
            registers::A |= prev_b++;
            registers::B = registers::C = registers::IP = 0;
            auto saved_a = registers::A;
            static size_t i = 0;
            while (registers::IP < commands.size()) {
                all_op_codes[commands[registers::IP]]->apply(commands[registers::IP + 1], output);
                registers::IP = registers::IP; // suppress a warning
            }

            if (output.str()[0] == '0' + next_commands.back()) {
                auto a = find_initial_a_register_to_print_the_program(next_commands.subspan(0, next_commands.size() - 1), saved_a, found);
                if (found) return a;
                output.str("");
            }
        }

        found = false;
        return 0;
    }
};

int first_part_2024_17() {
    program p;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&p](std::string str) {
        if (str.contains("Register")) p.parse_register(str);
        else if (str.contains("Program")) p.parse_command(str);
    });
    p.run(std::cout);
    std::cout << "\n";
    std::cout << "A: " << registers::A << " B: " << registers::B << " C: " << registers::C << "\n";
    return EXIT_SUCCESS;
}

int second_part_2024_17() {
    program p;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&p](std::string str) {
        if (str.contains("Register")) p.parse_register(str);
        else if (str.contains("Program")) p.parse_command(str);
    });

    registers::A = p.find_initial_a_register_to_print_the_program();
    std::cout << registers::A << "\n";

    registers::B = registers::C = registers::IP = 0;
    p.run(std::cout);
    std::cout << "\n";
    p.print_command();
    return EXIT_SUCCESS;
}

#endif
