#include <iostream>
#include <fstream>
#include "main_definitions_helper.hpp"

#define YEAR 2020
#define DAY 17
#define PART first
#define TEST_RUN true

#if TEST_RUN
    #define INPUT_SOURCE cin
#else
    #define INPUT_SOURCE std::cin
#endif

#include INCLUDE_FILE(YEAR, DAY)

int main() {
    CALL_DESIRED_FUNCTION(PART, YEAR, DAY);
    return 0;
}