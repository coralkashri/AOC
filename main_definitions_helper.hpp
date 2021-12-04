#ifndef ADVENTOFCODE2021_MAIN_DEFINITIONS_HELPER_HPP
#define ADVENTOFCODE2021_MAIN_DEFINITIONS_HELPER_HPP

// Macro for adding quotes
#define STRINGIFY(X) STRINGIFY2(X)
#define STRINGIFY2(X) #X

// Macros for concatenating tokens
#define CAT(X,Y) CAT2(X,Y)
#define CAT2(X,Y) X##Y
#define CAT_2 CAT
#define CAT_3(X,Y,Z) CAT(X,CAT(Y,Z))
#define CAT_4(A,X,Y,Z) CAT(A,CAT_3(X,Y,Z))
#define CAT_5(B, A,X,Y,Z) CAT(B,CAT_4(A,X,Y,Z))

#define FILE_PATH(year, day) CAT(year/day_, day.hpp)
#define INCLUDE_FILE(year, day) STRINGIFY(FILE_PATH(year, day))
#define CALL_DESIRED_FUNCTION(part, year, day) CAT_5(part, _part_, year, _, day)()

#endif //ADVENTOFCODE2021_MAIN_DEFINITIONS_HELPER_HPP