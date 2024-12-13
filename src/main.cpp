#include <iostream>
#include "main_definitions_helper.hpp"
#include <sys/resource.h>
#include <vector>
#include <chrono>
#include <execution>

#define YEAR 2024
#define DAY 11
//#define PART first
#define PART second
#define USE_TEST_CASE false
#define TEST_RUN true

#if TEST_RUN
    #if USE_TEST_CASE
        #define FILE_NAME test_run_
    #else
        #define FILE_NAME real_run_
    #endif

    #define INPUT_SOURCE_FILE_NAME STRINGIFY(CAT_4(../input_files/FILE_NAME, YEAR, _, DAY)) ".txt"

    #define INPUT_SOURCE cin
#else
    #define INPUT_SOURCE std::cin
#endif

#include INCLUDE_FILE(YEAR, DAY)
#include "tools/time_extensions.hpp"

int main() {
    id_t pid = getpid();
    //int ret = setpriority(PRIO_PROCESS, pid, NZERO - 1);

    if (!INPUT_SOURCE.is_open()) {
        std::cout << "Can't open file: " INPUT_SOURCE_FILE_NAME;
        return 1;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    CALL_DESIRED_FUNCTION(PART, YEAR, DAY);
    auto stop_time = std::chrono::high_resolution_clock::now();
    std::cout << "Computation time: ";
    pretty_time_print(start_time, stop_time);
    std::cout << std::endl;
    return 0;
}