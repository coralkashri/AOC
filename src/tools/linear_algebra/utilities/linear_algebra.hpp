#ifndef LINEARALGEBRACURSE_LINEARALGEBRA_H
#define LINEARALGEBRACURSE_LINEARALGEBRA_H

#include <iostream>
#include <cmath>
#include <vector>
#include "../exceptions.hpp"

namespace LinearAlgebra {
    size_t pow(size_t a, size_t b);

    /**
     *
     * @param a
     * @param b
     * @param absEpsilon - For zero equalization
     * @param relEpsilon
     *
     * @return true if the difference between a && b is less than absEpsilon, or within relEpsilon percent of the larger of a && b
     */
    bool approximatelyEqualAbsRel(double a, double b, double absEpsilon = 1e-6, double relEpsilon = 1e-8);


};

/// Template functions
namespace LinearAlgebra {

    /**
     *
     * @param vec
     *
     * @throw
     *
     * @return
     */
    template<class T>
    size_t first_nonzero_index(const std::vector<T> &vec) {
        for (size_t i = 0; i < vec.size(); i++) {
            if (!approximatelyEqualAbsRel(vec[i], 0)) {
                return i;
            }
        }
        throw NoNonzeroElementInVecException();
    }

    template<class T>
    size_t last_nonzero_index(const std::vector<T> &vec) {
        for (int i = vec.size() - 1; i >= 0; i--) {
            if (!approximatelyEqualAbsRel(vec[i], 0)) {
                return i;
            }
        }
        throw NoNonzeroElementInVecException();
    }
}

#endif //LINEARALGEBRACURSE_LINEARALGEBRA_H
