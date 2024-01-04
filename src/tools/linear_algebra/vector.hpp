#ifndef LINEARALGEBRACURSE_VECTOR_H
#define LINEARALGEBRACURSE_VECTOR_H

#include <iostream>
#include <vector>
#include <cmath>
#include "utilities/linear_algebra.hpp"
#include "exceptions.hpp"

namespace LinearAlgebra {

    class vector {
    private:
        std::vector<double> orientations;

        double sum();
        bool equal_orientations();
        vector operator/(const vector& vec) const;

    public:
        static size_t precision;
        explicit vector(const std::vector<double> &orientations);
        vector(const vector &vec);
        size_t dimension() const;
        std::vector<double> get_orientations() const;

        double magnitude() const;
        vector normalized_vector() const;
        bool is_zero() const;
        double angle_with(const vector &vec, bool in_degrees = false) const;
        double dot(const vector &vec) const;
        bool is_parallel_to(const vector &vec) const;
        bool is_orthogonal_to(const vector &vec) const;
        vector component_parallel_to(const vector &vec) const;
        vector component_orthogonal_to(const vector &vec) const;
        std::vector<vector> compute_by_basis_vec(const vector &vec) const;
        vector cross(const vector &vec) const;
        double area_of_parallelogram(const vector &vec) const;
        double area_of_triangle(const vector &vec) const;

        vector operator+(const vector& vec) const;
        vector operator-(const vector& vec) const;
        vector operator*(const vector& vec) const;
        vector operator*=(const vector& vec);
        vector operator*(double scalar) const;
        bool operator==(const vector& vec) const;

        double operator[](size_t orientation_dim) const;

        vector erase(size_t place);

        friend std::ostream& operator<<(std::ostream& os, const vector& vec);
        friend vector operator*(double scalar, const vector& vec);
    };

}


#endif //LINEARALGEBRACURSE_VECTOR_H
