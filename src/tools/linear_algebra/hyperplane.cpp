#include "hyperplane.hpp"

namespace LinearAlgebra {

    hyperplane::hyperplane(vector normal_vector, double constant_term) : normal_vector(normal_vector), constant_term(constant_term), base_point({0}) {
        dimension = normal_vector.dimension();
        set_basepoint();
    }

    hyperplane &hyperplane::operator=(const hyperplane &p) {
        //if (p.dimension != dimension) throw hyperplanesInDifferentDimException();
        this->dimension = p.dimension;
        this->normal_vector = p.normal_vector;
        this->constant_term = p.constant_term;
        this->base_point = p.base_point;
        return *this;
    }

    std::string hyperplane::write_coefficient(double coefficient, bool is_initial_term) {
        std::stringstream output;
        if (coefficient < 0) output << "-";
        else if (coefficient > 0 && !is_initial_term) output << "+";
        if (!is_initial_term) output << " ";
        if (std::abs(coefficient) != 1) output << std::abs(coefficient);
        return output.str();
    }

    void hyperplane::set_basepoint() {
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

    bool hyperplane::is_parallel_to(const hyperplane &p) const {
        return normal_vector.is_parallel_to(p.normal_vector);
    }

    vector hyperplane::get_normal_vector() const {
        return normal_vector;
    }

    size_t hyperplane::get_dimension() const {
        return dimension;
    }

    double hyperplane::get_constant_term() const {
        return constant_term;
    }

    hyperplane &hyperplane::operator*=(const hyperplane &p) {
        normal_vector *= p.normal_vector;
        constant_term *= p.constant_term;
        set_basepoint();
        return *this;
    }

    hyperplane &hyperplane::operator*=(double constant) {
        normal_vector = normal_vector * constant;
        constant_term *= constant;
        set_basepoint();
        return *this;
    }

    hyperplane &hyperplane::operator+=(const hyperplane &p) {
        normal_vector = normal_vector + p.normal_vector;
        constant_term += p.constant_term;
        set_basepoint();
        return *this;
    }

    hyperplane &hyperplane::operator-=(const hyperplane &p) {
        normal_vector = normal_vector - p.normal_vector;
        constant_term -= p.constant_term;
        set_basepoint();
        return *this;
    }

    hyperplane hyperplane::operator*(const hyperplane &p) const {
        hyperplane res(*this);
        res.normal_vector *= p.normal_vector;
        res.constant_term *= p.constant_term;
        res.set_basepoint();
        return res;
    }

    hyperplane hyperplane::operator*(double constant) const {
        hyperplane res(*this);
        res.normal_vector = res.normal_vector * constant;
        res.constant_term *= constant;
        res.set_basepoint();
        return res;
    }

    hyperplane hyperplane::operator+(const hyperplane &p) const {
        hyperplane res(*this);
        res.normal_vector = res.normal_vector + p.normal_vector;
        res.constant_term += p.constant_term;
        res.set_basepoint();
        return res;
    }

    hyperplane hyperplane::operator-(const hyperplane &p) const {
        hyperplane res(*this);
        res.normal_vector = res.normal_vector - p.normal_vector;
        res.constant_term -= p.constant_term;
        res.set_basepoint();
        return res;
    }

    bool hyperplane::operator==(const hyperplane &p) const {
        bool res = false;
        if (this->is_parallel_to(p)) {
            vector vec_between_lines = this->base_point - p.base_point;
            if (vec_between_lines.is_orthogonal_to(this->normal_vector)) {
                res = true;
            }
        }
        return res;
    }

    std::ostream &operator<<(std::ostream &os, const hyperplane &p) {
        try {
            std::vector<double> n = p.normal_vector.get_orientations();
            size_t initial_idx = LinearAlgebra::first_nonzero_index(n);
            for (size_t i = 0; i < n.size(); i++) {
                if (!LinearAlgebra::approximatelyEqualAbsRel(n[i], 0)) {
                    os << hyperplane::write_coefficient(n[i], i == initial_idx) << " x_" << std::to_string(i + 1) << " ";
                }
            }
        } catch (NoNonzeroElementInVecException &e) {
            os << "0";
        } catch (std::exception &e) {
            throw e;
        } catch (...) {
            throw std::runtime_error("Unexpected error occurred.");
        }
        os << " = " << p.constant_term;
        return os;
    }
}