#ifndef ADVENTOFCODE2021_WORD_DELIMITED_BY_H
#define ADVENTOFCODE2021_WORD_DELIMITED_BY_H


template<char delimiter>
class WordDelimitedBy : public std::string{};

template <char delimiter>
std::istream& operator>>(std::istream& is, WordDelimitedBy<delimiter>& output) {
    std::getline(is, output, delimiter);
    return is;
}

#endif //ADVENTOFCODE2021_WORD_DELIMITED_BY_H