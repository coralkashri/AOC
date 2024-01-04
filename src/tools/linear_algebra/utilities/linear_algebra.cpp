#include "linear_algebra.hpp"

namespace LinearAlgebra {
    size_t pow(size_t a, size_t b) {
        size_t res = 1;
        while (b--) {
            res *= a;
        }
        return res;
    }

    bool approximatelyEqualAbsRel(double a, double b, double absEpsilon, double relEpsilon)
    {
        if ((std::isinf(a) && std::isinf(b)) || (std::isnan(a) && std::isnan(b))) return true;
        // Check if the numbers are really close -- needed when comparing numbers near zero.
        double diff = fabs(a - b);
        if (diff <= absEpsilon) // For zero equalization
            return true;

        // Otherwise fall back to Knuth's algorithm
        return diff <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * relEpsilon);
    }
}