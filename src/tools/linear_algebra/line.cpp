#include "line.hpp"

namespace LinearAlgebra {

    line::line(vector normal_vector, double constant_term) : normal_vector(normal_vector), constant_term(constant_term), base_point({0}) {
        dimension = 2;
        if (normal_vector.dimension() != dimension) {
            this->normal_vector = vector(std::vector<double>(dimension, 0.0));
        }
        set_basepoint();
    }

    std::string line::write_coefficient(double coefficient, bool is_initial_term) {
        std::string output;
        if (coefficient < 0) output += "-";
        else if (coefficient > 0 && !is_initial_term) output += "+";
        if (!is_initial_term) output += " ";
        if (std::abs(coefficient) != 1) output += std::to_string(std::abs(coefficient));
        return output;
    }

    vector line::get_direction_vector() const {
        return vector({this->normal_vector[1], -this->normal_vector[0]});
    }

    void line::set_basepoint() {
        try {
            size_t initial_idx = LinearAlgebra::first_nonzero_index(normal_vector.get_orientations());
            double initial_coefficient = normal_vector.get_orientations()[initial_idx];
            std::vector<double> basepoint_coords(dimension, 0);
            basepoint_coords[initial_idx] = constant_term / initial_coefficient;
            base_point = vector(basepoint_coords);
        } catch (NoNonzeroElementInVecException &e) {
            base_point = vector(std::vector<double>(dimension, 0));
        } catch (std::exception &e) {
            throw e;
        } catch (...) {
            throw std::runtime_error("Unexpected error occurred.");
        }
    }

    bool line::is_parallel_to(const line &l) const {
        return normal_vector.is_parallel_to(l.normal_vector);
    }

    vector line::intersection_point_with(const line &l) const {
        if (is_parallel_to(l)) {
            if (*this == l) {
                throw InfiniteIntersectionsPointsException();
            }
            throw NoIntersectionPointException();
        }
        bool switch_master_line = LinearAlgebra::approximatelyEqualAbsRel(this->normal_vector[0], 0);
        line master_line = switch_master_line ? l : *this;
        line second_line = switch_master_line ? *this : l;

        double A, B, C, D;
        A = master_line.normal_vector[0];
        B = master_line.normal_vector[1];
        C = second_line.normal_vector[0];
        D = second_line.normal_vector[1];

        double x = (D * master_line.constant_term - B * second_line.constant_term) / (A * D - B * C);
        double y = (-C * master_line.constant_term + A * second_line.constant_term) / (A * D - B * C);
        vector intersection_point({x, y});

        return intersection_point;
    }

    bool line::operator==(const line &l) const {
        bool res = false;
        if (this->is_parallel_to(l)) {
            vector vec_between_lines = this->base_point - l.base_point;
            if (vec_between_lines.is_orthogonal_to(this->normal_vector)) {
                res = true;
            }
        }

        /*res = false;
        start = std::chrono::high_resolution_clock::now();
        vector vec_between_lines = this->base_point - l.base_point;
        if (!vec_between_lines.magnitude()) {
            vec_between_lines = vec_between_lines + this->get_direction_vector();
        }
        if (vec_between_lines.is_orthogonal_to(this->normal_vector) &&
            vec_between_lines.is_orthogonal_to(l.normal_vector)) {
            res = true;
        }
        stop = std::chrono::high_resolution_clock::now();
        std::cout << "Second checking time [ns]: " << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() << std::endl;*/

        return res;
    }

    std::ostream &operator<<(std::ostream &os, const line &l) {
        try {
            std::vector<double> n = l.normal_vector.get_orientations();
            size_t initial_idx = LinearAlgebra::first_nonzero_index(n);
            for (size_t i = 0; i < n.size(); i++) {
                if (!LinearAlgebra::approximatelyEqualAbsRel(n[i], 0)) {
                    os << line::write_coefficient(n[i], i == initial_idx) << " x_" << std::to_string(i + 1) << " ";
                }
            }
        } catch (NoNonzeroElementInVecException &e) {
            os << "0";
        } catch (std::exception &e) {
            throw e;
        } catch (...) {
            throw std::runtime_error("Unexpected error occurred.");
        }
        os << " = " << std::to_string(l.constant_term);
        return os;
    }
}