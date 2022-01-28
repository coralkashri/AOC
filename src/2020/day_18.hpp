#ifndef AOC_2020_18
#define AOC_2020_18

#include "../tools/base_includes.h"

efficient_stack<char, 150> infix_to_postfix_stack;
efficient_stack<double, 150> solve_postfix_stack;

std::string infix_to_postfix(std::string::iterator infix, auto &&preced) {
    std::string postfix;
    for (; *infix; ++infix) {
        if (*infix == ' ') continue;
        if (*infix >= '0' && *infix <= '9')
            postfix += *infix;
        else if (*infix == '(')
            infix_to_postfix_stack.push(*infix);
        else if (*infix == ')') {
            while (!infix_to_postfix_stack.empty() && infix_to_postfix_stack.top() != '(') {
                postfix += infix_to_postfix_stack.pop();
            }
            infix_to_postfix_stack.pop(); // Remove the '(' from stack
        } else {
            if (infix_to_postfix_stack.empty() || preced(*infix) > preced(infix_to_postfix_stack.top()))
                infix_to_postfix_stack.push(*infix); // Push if precedence is high
            else {
                while (!infix_to_postfix_stack.empty() && preced(*infix) <= preced(infix_to_postfix_stack.top())) {
                    postfix += infix_to_postfix_stack.pop(); // Store and pop until higher precedence is found
                }
                infix_to_postfix_stack.push(*infix);
            }
        }
    }

    while (!infix_to_postfix_stack.empty()) {
        postfix += infix_to_postfix_stack.pop();
    }

    return postfix;
}

auto solve_postfix(std::string::iterator postfix_exp) {
    for (; *postfix_exp; ++postfix_exp) {
        if (*postfix_exp >= '0' && *postfix_exp <= '9') solve_postfix_stack.push(*postfix_exp - '0');
        else {
             auto n1 = solve_postfix_stack.pop();
             auto n2 = solve_postfix_stack.pop();
            switch (*postfix_exp) {
                case '+': solve_postfix_stack.push(n1 + n2);
                    break;
                case '-': solve_postfix_stack.push(n1 - n2);
                    break;
                case '*': solve_postfix_stack.push(n1 * n2);
                    break;
                case '/': solve_postfix_stack.push(n1 / n2);
                    break;
            }
        }
    }
    return solve_postfix_stack.pop();
}

int first_part_2020_18() {
    auto preced = [](char c) {
            if (c == '+' || c == '-' || c == '*' || c == '/') return 1;
            return 0;
    };
    std::cout << std::transform_reduce(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), 0ull, std::plus<>{}, [&] (std::string str) mutable {
        return solve_postfix(infix_to_postfix(str.begin(), preced).begin());
    }) << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2020_18() {
    auto preced = [](char c) {
        if (c == '*' || c == '/') return 1;
        if (c == '+' || c == '-') return 2;
        return 0;
    };
    std::cout << std::transform_reduce(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), 0ull, std::plus<>{}, [&] (std::string str) mutable {
        return solve_postfix(infix_to_postfix(str.begin(), preced).begin());
    }) << std::endl;

    return EXIT_SUCCESS;
}

#endif