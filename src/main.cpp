#include <iostream>
#include <fstream>
#include "main_definitions_helper.hpp"
#include <sys/resource.h>

#define YEAR 2020
#define DAY 15
#define PART first
#define TEST_RUN true

#if TEST_RUN
    #define INPUT_SOURCE cin
#else
    #define INPUT_SOURCE std::cin
#endif

#include INCLUDE_FILE(YEAR, DAY)
#include "tools/time_extensions.hpp"

int main() {
    id_t pid = getpid();
    int ret = setpriority(PRIO_PROCESS, pid, NZERO - 1);

    auto start_time = std::chrono::high_resolution_clock::now();
    CALL_DESIRED_FUNCTION(PART, YEAR, DAY);
    auto stop_time = std::chrono::high_resolution_clock::now();
    std::cout << "Computation time: ";
    pretty_time_print(start_time, stop_time);
    std::cout << std::endl;
    return 0;
}