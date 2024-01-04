#ifndef LINEARALGEBRACURSE_hyperplane_H
#define LINEARALGEBRACURSE_hyperplane_H

#include <iomanip>
#include "vector.hpp"

namespace LinearAlgebra {

    class hyperplane {
    private:
        vector normal_vector;
        vector base_point;
        double constant_term;
        size_t dimension;

        static std::string write_coefficient(double coefficient, bool is_initial_term = false);

        //vector get_direction_vector() const;

    public:
        hyperplane(vector normal_vector = vector({0}), double constant_term = 0);

        hyperplane(const hyperplane &l) = default;

        void set_basepoint();

        bool is_parallel_to(const hyperplane &p) const;

        vector get_normal_vector() const;

        size_t get_dimension() const;

        double get_constant_term() const;

        //vector intersection_point_with(const hyperplane &l) const;

        hyperplane& operator=(const hyperplane &p);

        hyperplane& operator*=(const hyperplane &p);

        hyperplane& operator*=(double constant);

        hyperplane& operator+=(const hyperplane &p);

        hyperplane& operator-=(const hyperplane &p);

        hyperplane operator*(const hyperplane &p) const;

        hyperplane operator*(double constant) const;

        hyperplane operator+(const hyperplane &p) const;

        hyperplane operator-(const hyperplane &p) const;

        bool operator==(const hyperplane &p) const;

        friend std::ostream &operator<<(std::ostream &os, const hyperplane &l);
    };
}


#endif //LINEARALGEBRACURSE_hyperplane_H
