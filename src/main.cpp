#include <iostream>
#include <fstream>
#include "main_definitions_helper.hpp"

#define YEAR 2020
#define DAY 17
#define PART second
#define TEST_RUN true

#if TEST_RUN
    #define INPUT_SOURCE cin
#else
    #define INPUT_SOURCE std::cin
#endif

#include INCLUDE_FILE(YEAR, DAY)

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();
    CALL_DESIRED_FUNCTION(PART, YEAR, DAY);
    auto stop_time = std::chrono::high_resolution_clock::now();
    std::cout << "Computation time: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time).count() << "ms" << std::endl;
    return 0;
}