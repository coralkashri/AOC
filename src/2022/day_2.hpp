#ifndef AOC_2022_2
#define AOC_2022_2

#include <iostream>
#include <iterator>
#include <bitset>
#include <vector>
#include <list>
#include "../input_source.hpp"

int first_part_2022_2() {
    std::cout << std::accumulate(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), 0ull, [] (size_t number, std::string str) mutable {
        char opponent = str[0];
        switch ((str[2] - opponent) * (opponent - 'A' + 1)) {
            case 44: return number + 1ull;
            case 66: return number + 2ull;
            case 25: return number + 3ull;
            case 23: return number + 4ull;
            case 46: return number + 5ull;
            case 69: return number + 6ull;
            case 63: return number + 7ull;
            case 24: return number + 8ull;
            case 48: return number + 9ull;
            default: std::cout << "Error! " << str[0] << " " << str[2] << " " << ((str[2] - opponent) * (opponent - 'A' + 1)) <<  std::endl; return 0ull;
        }
        return 0ull;
    }) << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2022_2() {
    std::cout << std::accumulate(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), 0ull, [] (size_t current_score, std::string str) mutable {
        //                                     priorities['Z' - 'X' = 2 + 1] = 3
        //                                     priorities['Z' - 'Y' = 1 + 1] = 2
        //                                     priorities['Z' - 'Z' = 0 + 1] = 1
        //                                     X >Z >Y >X >Z
        static std::vector<int> priorities = { 1, 3, 2, 1, 3 };
        
        // If the opponent is Rock (X) and we want for example to win, we need to take the tool that will make Rock to lose.
        // In priorities Rock(X) is priorities[3], and Paper(Y) is priorities[2].
        // So we get the rock location and -1 for it to win it.
        // If we want to lose to it, we'll take Rock(X) location + 1, priorities[4] which is Scissors(Z).
        
        int opponent = str[0] - 'A'; // 0(A) - rock, 1(B) - paper, 2(C) - scissors
        int result = 'Z' - str[2]; // 0(Z) - win, 1(Y) - tie, 2(X) - lose
        int relative_location_for_result = result - 1; // -1 - win, 0 - tie, 1 - lose
        int result_score = (2 - result) * 3;
        
        // A - A = 0 = Rock = priorities[3]
        // B - A = 1 = Paper = priorities[2]
        // C - A = 2 = Scissors = priorities[1]
		int opponent_priorities_index = 3 - opponent;
        
        return current_score + priorities[opponent_priorities_index + relative_location_for_result] + result_score;
    }) << std::endl;

    return EXIT_SUCCESS;
}

#endif