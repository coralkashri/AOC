#ifndef ADVENTOFCODE2021_GET_LINE_STREAM_H
#define ADVENTOFCODE2021_GET_LINE_STREAM_H

#include <sstream>

bool get_line_stream(std::istream &input, std::stringstream &output, char delimiter) {
    std::string temp_line;
    auto &res = std::getline(input, temp_line, delimiter);
    output.clear();
    output.str(temp_line);
    return (bool)res;
}

bool get_line_stream(std::istream &input, std::stringstream &output) {
    return get_line_stream(input, output, input.widen('\n'));
}

#endif