#ifndef LINEARALGEBRACURSE_plane_H
#define LINEARALGEBRACURSE_plane_H

#include <iomanip>
#include "vector.hpp"

namespace LinearAlgebra {

    class plane {
    private:
        vector normal_vector;
        vector base_point;
        double constant_term;
        size_t dimension;

        static std::string write_coefficient(double coefficient, bool is_initial_term = false);

        //vector get_direction_vector() const;

    public:
        plane(vector normal_vector = vector({0}), double constant_term = 0);

        plane(const plane &l) = default;

        void set_basepoint();

        bool is_parallel_to(const plane &p) const;

        vector get_normal_vector() const;

        size_t get_dimension() const;

        double get_constant_term() const;

        //vector intersection_point_with(const plane &l) const;

        plane& operator=(const plane &p);

        plane& operator*=(const plane &p);

        plane& operator*=(double constant);

        plane& operator+=(const plane &p);

        plane& operator-=(const plane &p);

        plane operator*(const plane &p) const;

        plane operator*(double constant) const;

        plane operator+(const plane &p) const;

        plane operator-(const plane &p) const;

        bool operator==(const plane &p) const;

        friend std::ostream &operator<<(std::ostream &os, const plane &l);
    };
}


#endif //LINEARALGEBRACURSE_plane_H
