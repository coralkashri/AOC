#ifndef LINEARALGEBRACURSE_LINE_H
#define LINEARALGEBRACURSE_LINE_H

#include <iostream>
#include <chrono>
#include "vector.hpp"
#include "utilities/linear_algebra.hpp"

namespace LinearAlgebra {

    class line {
    private:
        vector normal_vector;
        vector base_point;
        double constant_term;
        size_t dimension;

        static std::string write_coefficient(double coefficient, bool is_initial_term = false);
        vector get_direction_vector() const;

    public:
        line(vector normal_vector, double constant_term = 0);
        line(const line &l) = default;
        void set_basepoint();

        bool is_parallel_to(const line& l) const;
        vector intersection_point_with(const line &l) const;
        bool operator==(const line& l) const;

        friend std::ostream& operator<<(std::ostream &os, const line &l);
    };

}

#endif //LINEARALGEBRACURSE_LINE_H