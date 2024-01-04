#include "vector.hpp"

namespace LinearAlgebra {

    size_t vector::precision = pow(10, 4);

    vector::vector(const std::vector<double> &orientations) : orientations(orientations) {
        if (orientations.empty()) throw std::runtime_error("vector::vector() ::=> orientations count is 0.");
    }

    vector::vector(const vector &vec) {
        orientations = vec.orientations;
    }

    double vector::sum() {
        auto it = orientations.begin();
        double sum = 0;
        while (it != orientations.end()) {
            sum += *it;
            it++;
        }
        return sum;
    }

    bool vector::equal_orientations() {
        auto it = orientations.begin();
        double first_orientation = *it;
        while (it != orientations.end()) {
            if (!approximatelyEqualAbsRel(*it, first_orientation))
                return false;
            it++;
        }
        return true;
    }

    size_t vector::dimension() const {
        return orientations.size();
    }

    std::vector<double> vector::get_orientations() const {
        return orientations;
    }

    double vector::magnitude() const {
        double pows_sum = 0;
        for (auto &orient: orientations) {
            pows_sum += orient * orient;
        }
        return std::sqrt(pows_sum);
    }

    vector vector::normalized_vector() const {
        if (!(bool)magnitude())
            throw NormalizeZeroVectorException();
        return (1 / magnitude()) * *this;
    }

    bool vector::is_zero() const {
        for (auto &orient : orientations)
            if (!LinearAlgebra::approximatelyEqualAbsRel(orient, 0)) return false;
        return true;
    }

    double vector::angle_with(const vector &vec, bool in_degrees) const {
        double ans;
        try {
            /*ans = std::acos(this->dot(vec) / (this->magnitude() * vec.magnitude())); // Option 1
            if (std::isnan(ans)) throw NormalizeZeroVectorException(); // Option 1*/
            double ndn = this->normalized_vector().dot(vec.normalized_vector());
            ndn = floor(ndn * 100000000) / 100000000;
            ans = std::acos(ndn); // Option 2
            ans = floor(ans * 1000) / 1000;
        } catch (NormalizeZeroVectorException &e) {
            throw AngleWithZeroVectorException();
        } catch (std::exception &e) {
            throw e;
        } catch (...) {
            throw std::runtime_error("Unexpected error occurred.");
        }
        return in_degrees ? ans : ans * 180.0 / M_PI;
    }

    double vector::dot(const vector &vec) const {
        vector multiplication_vec(*this);
        multiplication_vec *= vec;
        return multiplication_vec.sum();
    }

    bool vector::is_parallel_to(const vector &vec) const {
        bool ans = is_zero() || vec.is_zero() || angle_with(vec) == 0 || angle_with(vec) == M_PI;
        return ans;
    }

    bool vector::is_orthogonal_to(const vector &vec) const {
        return approximatelyEqualAbsRel(this->dot(vec), 0);
    }

    vector vector::component_parallel_to(const vector &vec) const {
        try {
            auto norm_vec = vec.normalized_vector();
            return this->dot(norm_vec) * norm_vec;

        } catch (NormalizeZeroVectorException &e) {
            throw NoUniqueParallelComponentException();
        } catch (std::exception &e) {
            throw e;
        } catch (...) {
            throw std::runtime_error("Unexpected error occurred.");
        }
    }

    vector vector::component_orthogonal_to(const vector &vec) const {
        try {
            auto proj = this->component_parallel_to(vec);
            return *this - proj;

        } catch (NoUniqueParallelComponentException &e) {
            throw NoUniqueOrthogonalComponentException();
        } catch (std::exception &e) {
            throw e;
        } catch (...) {
            throw std::runtime_error("Unexpected error occurred.");
        }
    }

    std::vector<vector> vector::compute_by_basis_vec(const vector &vec) const {
        std::vector<vector> res = {this->component_parallel_to(vec), this->component_orthogonal_to(vec)};
        return res;
    }

    vector vector::cross(const vector &vec) const {
        try {
            if (this->dimension() < 2 || this->dimension() > 3)
                throw NoCrossProductForDimensionException();
            if (this->dimension() == 2) {
                throw Needed3DimForCrossProductException();
            }
            vector cross_product_vec({0, 0, 0});
            cross_product_vec.orientations[0] = (*this)[1] * vec[2] - vec[1] * (*this)[2];
            cross_product_vec.orientations[1] = -((*this)[0] * vec[2] - vec[0] * (*this)[2]);
            cross_product_vec.orientations[2] = (*this)[0] * vec[1] - vec[0] * (*this)[1];
            if (!cross_product_vec.is_orthogonal_to(*this) || !cross_product_vec.is_orthogonal_to(vec))
                throw SomethingWrongInCrossProductException();
            return cross_product_vec;

        } catch (Needed3DimForCrossProductException &e) {
            vector self_in_r3(*this);
            vector vec_in_r3(vec);
            self_in_r3.orientations.emplace_back(0);
            vec_in_r3.orientations.emplace_back(0);
            return self_in_r3.cross(vec_in_r3);
        }
    }

    double vector::area_of_parallelogram(const vector &vec) const {
        try {
            auto res = this->cross(vec).magnitude();
            return res;

        } catch (NoCrossProductForDimensionException &e) {
            throw CantComputeParallelogramException(e);
        } catch (SomethingWrongInCrossProductException &e) {
            throw CantComputeParallelogramException(e);
        } catch (std::exception &e) {
            throw e;
        } catch (...) {
            throw std::runtime_error("Unexpected error occurred.");
        }
    }

    double vector::area_of_triangle(const vector &vec) const {
        try {
            auto res = this->cross(vec).magnitude() / 2;
            return res;

        } catch (CantComputeParallelogramException &e) {
            throw e;
        } catch (std::exception &e) {
            throw e;
        } catch (...) {
            throw std::runtime_error("Unexpected error occurred.");
        }
    }

    vector vector::operator+(const vector &vec) const {
        vector new_vec(*this);
        for (size_t i = 0; i < vec.dimension(); i++) {
            new_vec.orientations[i] += vec.orientations[i];
        }
        return new_vec;
    }

    vector vector::operator-(const vector &vec) const {
        vector new_vec(*this);
        for (size_t i = 0; i < vec.dimension(); i++) {
            new_vec.orientations[i] -= vec.orientations[i];
        }
        return new_vec;
    }

    vector vector::operator*(const vector &vec) const {
        vector new_vec(*this);
        for (size_t i = 0; i < vec.dimension(); i++) {
            new_vec.orientations[i] *= vec.orientations[i];
        }
        return new_vec;
    }

    vector vector::operator*=(const vector &vec) {
        for (size_t i = 0; i < vec.dimension(); i++) {
            orientations[i] = orientations[i] * vec.orientations[i];
        }
        return *this;
    }

    vector vector::operator*(double scalar) const {
        vector new_vec(*this);
        for (auto &orient : new_vec.orientations) {
            orient *= scalar;
        }
        return new_vec;
    }

    bool vector::operator==(const vector &vec) const {
        bool flag = vec.dimension() == this->dimension();
        for (size_t i = 0; i < vec.dimension(); i++) {
            if (!flag) break;
            flag = flag && !(bool)(vec.orientations[i] - orientations[i]);
        }
        return flag;
    }

    vector vector::operator/(const vector &vec) const {
        vector new_vec(*this);
        for (size_t i = 0; i < vec.dimension(); i++) {
            new_vec.orientations[i] /= vec.orientations[i];
        }
        return new_vec;
    }

    std::ostream& operator<<(std::ostream &os, const vector &vec) {
        os << "(";
        for (auto &orientation : vec.orientations) {
            os << orientation;
            if (&orientation != &*(vec.orientations.end() - 1)) os << ", ";
        }
        os << ")";
        return os;
    }

    vector operator*(double scalar, const vector& vec) {
        vector new_vec(vec);
        for (auto &orient : new_vec.orientations) {
            orient *= scalar;
        }
        return new_vec;
    }

    double vector::operator[](size_t orientation_dim) const {
        if (orientation_dim >= orientations.size())
            throw VectorNotExistsInSpecifiedDimensionException();
        return orientations[orientation_dim];
    }

    vector vector::erase(size_t place) {
        vector res(*this);
        if (place < 0 || place >= res.orientations.size()) throw VectorNotExistsInSpecifiedDimensionException();
        res.orientations.erase(res.orientations.begin() + place);
        return res;
    }
}