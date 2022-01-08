#ifndef AOC_2020_12
#define AOC_2020_12

#include "../tools/base_includes.h"

int first_part_2020_12() {
    //                        E  S  W  N
    int current_location[] = {0, 0, 0, 0};
    int current_direction = 0;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (std::string str) mutable {
        int number = std::stoi(str.substr(1));
        if (str[0] == 'R' || str[0] == 'L') {
            if (str[0] == 'L') number = 360 - number;
            current_direction = (current_direction + number / 90) % 4;
            return;
        }
        uint target_direction = str[0] == 'E' ? 0 : str[0] == 'S' ? 1 : str[0] == 'W' ? 2 : str[0] == 'N' ? 3 : current_direction;
        uint opposite_direction = (target_direction + 2) % 4;
        current_location[target_direction] += std::max(number - current_location[opposite_direction], 0);
        current_location[opposite_direction] -= std::min(current_location[opposite_direction], number);
    });

    std::cout << std::reduce(current_location, current_location + 4) << std::endl;

    return EXIT_SUCCESS;
}

int second_part_2020_12() {
    //                        E   S  W  N
    int current_location[] = {0 , 0, 0, 0};
    int waypoints[] =        {10, 0, 0, 1};
    int current_main_direction = 0;
    std::for_each(std::istream_iterator<WordDelimitedBy<'\n'>>(INPUT_SOURCE), std::istream_iterator<WordDelimitedBy<'\n'>>(), [&] (std::string str) mutable {
        int number = std::stoi(str.substr(1));
        if (str[0] == 'R' || str[0] == 'L') {
            if (str[0] == 'R') number = 360 - number;
            current_main_direction = (current_main_direction + number / 90) % 4;
            return;
        }
        uint target_direction = str[0] == 'E' ? 0 : str[0] == 'S' ? 1 : str[0] == 'W' ? 2 : str[0] == 'N' ? 3 : 4;
        if (target_direction != 4) {
            target_direction = (target_direction + current_main_direction) % 4;
            uint opposite_direction = (target_direction + 2) % 4;
            waypoints[target_direction] += std::max(number - waypoints[opposite_direction], 0);
            waypoints[opposite_direction] -= std::min(waypoints[opposite_direction], number);
        } else {
            for (uint direction = 0; direction < 4; direction++) {
                uint opposite_direction = (direction + 2) % 4;
                size_t relevant_waypoint_for_direction = (direction + current_main_direction) % 4;
                current_location[direction] += std::max(number * waypoints[(relevant_waypoint_for_direction) % 4] - current_location[opposite_direction], 0);
                current_location[opposite_direction] -= std::min(current_location[opposite_direction], number * waypoints[(relevant_waypoint_for_direction) % 4]);
            }
        }
    });

    std::cout << std::reduce(current_location, current_location + 4) << std::endl;

    return EXIT_SUCCESS;
}

#endif