#include <algorithm>
#include "linear_system.hpp"

namespace LinearAlgebra {

    linear_system::linear_system(const std::vector<LinearAlgebra::hyperplane> &planes) {
        size_t d = planes[0].get_dimension();
        for (auto &p : planes) {
            if (p.get_dimension() != d) throw planesInDifferentDimException();
        }
        this->planes = planes;
        this->dimension = d;
    }

    void linear_system::assertion_idx_test(size_t idx) {
        if (idx >= size()) throw ThereIsNoSuchDimensionInSystem();
        if (idx < 0) throw std::runtime_error("Access to negative dimension has been denied.");
    }

    bool linear_system::swap_with_row_below_for_nonzero_coefficient_if_able(size_t lead_idx, size_t current_row_check) {
        std::vector<int> first_nonzero_idx = indices_of_first_nonzero_terms_in_each_row();
        for (size_t i = current_row_check + 1; i < size(); i++) {
            if (first_nonzero_idx[i] == lead_idx) {
                swap_rows(current_row_check, i);
                return true;
            }
        }
        return false;
    }

    void linear_system::clear_coefficients_below(size_t lead_idx, size_t curr_lead_row) {
        double leader_var_value = planes[curr_lead_row].get_normal_vector()[lead_idx];
        for (size_t i = curr_lead_row + 1; i < this->size(); i++) {
            double curr_row_in_lead_idx_value = planes[i].get_normal_vector()[lead_idx];
            if (!LinearAlgebra::approximatelyEqualAbsRel(curr_row_in_lead_idx_value, 0)) {
                this->add_multiple_times_row_to_row(-curr_row_in_lead_idx_value / leader_var_value, lead_idx, i);
            }
        }
    }

    void linear_system::clear_coefficients_above(size_t lead_idx, size_t curr_lead_row) {
        double leader_var_value = planes[curr_lead_row].get_normal_vector()[lead_idx];
        for (int i = (int)curr_lead_row - 1; i >= 0; i--) {
            double curr_row_in_lead_idx_value = planes[i].get_normal_vector()[lead_idx];
            if (!LinearAlgebra::approximatelyEqualAbsRel(curr_row_in_lead_idx_value, 0)) {
                this->add_multiple_times_row_to_row(-curr_row_in_lead_idx_value / leader_var_value, curr_lead_row, i);
            }
        }
    }

    void linear_system::swap_rows(size_t row1, size_t row2) {
        assertion_idx_test(row1);
        assertion_idx_test(row2);
        hyperplane temp(vector({0}));
        temp = planes[row1];
        planes[row1] = planes[row2];
        planes[row2] = temp;
    }

    void linear_system::multiply_coefficient_and_row(double coefficient, size_t row) {
        assertion_idx_test(row);
        hyperplane res = planes[row] * coefficient;
        planes[row] = hyperplane(res.get_normal_vector(), res.get_constant_term());
    }

    void linear_system::add_multiple_times_row_to_row(double coefficient, size_t row_to_add, size_t row_to_be_added_to) {
        assertion_idx_test(row_to_add);
        assertion_idx_test(row_to_be_added_to);
        hyperplane res = planes[row_to_be_added_to] + planes[row_to_add] * coefficient;
        planes[row_to_be_added_to] = hyperplane(res.get_normal_vector(), res.get_constant_term());
    }

    linear_system linear_system::compute_triangular_form() {
        linear_system res(*this);

        for (size_t lead_variable_idx = 0, current_row = 0; current_row < res.size() && lead_variable_idx < res.dimension; current_row++, lead_variable_idx++) {
            while (lead_variable_idx < res.dimension && LinearAlgebra::approximatelyEqualAbsRel(res[current_row].get_normal_vector()[lead_variable_idx], 0)) {
                // Swap action might be needed.
                if (!res.swap_with_row_below_for_nonzero_coefficient_if_able(lead_variable_idx, current_row))
                    lead_variable_idx++;
                else break;
            }
            // Lead variable is non zero or there is no nonzero of this lead variable in the following rows

            if (lead_variable_idx >= res.dimension) break;

            // Make sure that all of the following rows in the current lead variable index are zeroes
            res.clear_coefficients_below(lead_variable_idx, current_row);
        }
        return res;
    }

    linear_system linear_system::compute_rref() {
        linear_system tf = this->compute_triangular_form();
        std::vector<int> pivot_indices = tf.indices_of_first_nonzero_terms_in_each_row();
        for (size_t current_row = 0; current_row < tf.size(); current_row++) {
            // Make sure that all of the following rows in the current lead variable index are zeroes
            int pivot_idx = pivot_indices[current_row];
            if (pivot_idx < 0) break;
            tf.multiply_coefficient_and_row(1 / tf[current_row].get_normal_vector()[pivot_idx], current_row);
            tf.clear_coefficients_above((size_t)pivot_idx, current_row);
        }
        return tf;
    }

    linear_system::Result linear_system::compute_ge_solution() {
        linear_system rref = this->compute_rref();
        Result res{SINGLE_SOLUTION, vector({0})};
        std::vector<int> nzero_idxs = rref.indices_of_first_nonzero_terms_in_each_row();
        size_t pivots_count = std::count_if(nzero_idxs.begin(), nzero_idxs.end(), [](int elem) -> bool { return elem != -1; });
        if (pivots_count < rref.dimension) res.type = MULTIPLE_SOLUTIONS;
        for (size_t i = pivots_count; i < rref.size(); i++) {
            if (!LinearAlgebra::approximatelyEqualAbsRel(rref[i].get_constant_term(), 0)) {
                res.type = NO_SOLUTION;
                break;
            }
        }
        if (res.type != NO_SOLUTION) {
            std::vector<double> result_vector_numbers(rref.dimension);
            size_t current_pivot = 0;
            for (size_t i = 0; i < pivots_count;) {
                if (LinearAlgebra::approximatelyEqualAbsRel(rref[i].get_normal_vector()[current_pivot], 0)) {
                    result_vector_numbers[current_pivot++] = 0;
                } else {
                    result_vector_numbers[current_pivot++] = rref[i].get_constant_term();
                    i++;
                }
            }
            res.res = vector(result_vector_numbers);
            if (res.type == MULTIPLE_SOLUTIONS) {
                compute_parametrization(res, rref);
            }
        }
        return res;
    }

    void linear_system::compute_parametrization(linear_system::Result &res, linear_system &rref) {
        std::vector<int> first_nzero_idxs = rref.indices_of_first_nonzero_terms_in_each_row();
        std::vector<int> last_nzero_idxs = rref.indices_of_last_nonzero_terms_in_each_row();
        for (int i = rref.size() - 1; i >= 0; i--) {
            if (first_nzero_idxs[i] == -1) continue;
            // if (first_nzero_idxs[i] == last_nzero_idxs[i]) => There is no free variables in this row.
            for (int j = last_nzero_idxs[i]; j > first_nzero_idxs[i]; j--) {
                std::vector<double> new_parametrize_vec(rref.dimension, 0);
                new_parametrize_vec[j] = 1;
                for (int k = i; k >= 0; k--) {
                    new_parametrize_vec[k] = -rref[k].get_normal_vector()[j];
                    while (LinearAlgebra::approximatelyEqualAbsRel(rref[k].get_normal_vector()[--last_nzero_idxs[k]], 0));
                }
                res.parametraize.emplace_back(new_parametrize_vec);
            }
        }
    }

    std::vector<int> linear_system::indices_of_first_nonzero_terms_in_each_row() {
        std::vector<int> indices(this->size(), -1);

        for (size_t i = 0; i < this->size(); i++) {
            try {
                indices[i] = (int)LinearAlgebra::first_nonzero_index(planes[i].get_normal_vector().get_orientations());
            } catch (NoNonzeroElementInVecException &e) {
                continue;
            } catch (std::exception &e) {
                throw e;
            } catch (...) {
                throw std::runtime_error("Unexpected error occurred.");
            }
        }

        return indices;
    }

    std::vector<int> linear_system::indices_of_last_nonzero_terms_in_each_row() {
        std::vector<int> indices(this->size(), -1);

        for (size_t i = 0; i < this->size(); i++) {
            try {
                indices[i] = (int)LinearAlgebra::last_nonzero_index(planes[i].get_normal_vector().get_orientations());
            } catch (NoNonzeroElementInVecException &e) {
                continue;
            } catch (std::exception &e) {
                throw e;
            } catch (...) {
                throw std::runtime_error("Unexpected error occurred.");
            }
        }

        return indices;
    }

    size_t linear_system::size() const {
        return planes.size();
    }

    hyperplane& linear_system::operator[](size_t idx) {
        assertion_idx_test(idx);
        return planes[idx];
    }

    std::ostream& operator<<(std::ostream &os, const linear_system &ls) {
        std::stringstream ss;
        ss << "Linear system:" << std::endl;
        for (size_t i = 0; i < ls.size(); i++) {
            ss << "Equation " << (i + 1) << ": " << ls.planes[i] << std::endl;
        }
        return os << ss.str();
    }
}
