#ifndef ADVENTOFCODE2021_BINARY_TOOLS_HPP
#define ADVENTOFCODE2021_BINARY_TOOLS_HPP

template <typename T>
void from_hex(const std::string& hexValue, T& result)
{
    std::stringstream ss;
    ss << std::hex << hexValue;
    ss >> result;
}

#endif
