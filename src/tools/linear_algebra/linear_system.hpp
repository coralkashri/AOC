#ifndef LINEARALGEBRACURSE_LINEAR_SYSTEM_H
#define LINEARALGEBRACURSE_LINEAR_SYSTEM_H

#include <iostream>
#include <vector>
#include <exception>
#include "utilities/linear_algebra.hpp"
#include "vector.hpp"
//#include "plane.hpp"
#include "hyperplane.hpp"

namespace LinearAlgebra {

    class linear_system {
    public:
        explicit linear_system(const std::vector<hyperplane> &planes);

        enum ResultsTypes {
            NO_SOLUTION = 0,
            SINGLE_SOLUTION = 1,
            MULTIPLE_SOLUTIONS = 2
        };

        struct Result {
            ResultsTypes type;
            vector res;
            std::vector<vector> parametraize;

            friend std::ostream& operator<<(std::ostream &os, const Result &res) {
                switch (res.type) {
                    case SINGLE_SOLUTION:
                        os << "Single solution: " << res.res;
                        break;

                    case MULTIPLE_SOLUTIONS:
                        os << "Multiple solutions:\n\r";
                        for (size_t dim = 0; dim < res.res.dimension(); dim++) {
                            os << "x_" << (dim + 1) << " = ";
                            if (!LinearAlgebra::approximatelyEqualAbsRel(res.res[dim], 0))
                                os << res.res[dim] << " ";
                            for (int param_idx = res.parametraize.size() - 1; param_idx >= 0; param_idx--) {
                                size_t current_param = 0;
                                for (size_t i = 0; i < res.parametraize[param_idx].get_orientations().size(); i++) {
                                    if (LinearAlgebra::approximatelyEqualAbsRel(res.parametraize[param_idx][i], 1)) {
                                        current_param = i + 1;
                                        break;
                                    }
                                }
                                if (!LinearAlgebra::approximatelyEqualAbsRel(res.parametraize[param_idx][dim], 0)) {
                                    if (!LinearAlgebra::approximatelyEqualAbsRel(res.parametraize[param_idx][dim], 1)) {
                                        if (res.parametraize[param_idx][dim] < 0) os << "- ";
                                        else os << "+ ";
                                        os << std::abs(res.parametraize[param_idx][dim]) << " ";
                                    }
                                    os << "x_" << current_param << " ";
                                }
                            }
                            os << "\n\r";
                        }
                        break;

                    case NO_SOLUTION:
                        os << "No solution.";
                        break;
                }
                return os;
            }
        };

        void swap_rows(size_t row1, size_t row2);
        void multiply_coefficient_and_row(double coefficient, size_t row);
        void add_multiple_times_row_to_row(double coefficient, size_t row_to_add, size_t row_to_be_added_to);
        linear_system compute_triangular_form();
        linear_system compute_rref();
        Result compute_ge_solution();
        std::vector<int> indices_of_first_nonzero_terms_in_each_row();
        std::vector<int> indices_of_last_nonzero_terms_in_each_row();
        size_t size() const;
        hyperplane& operator[](size_t idx);

        friend std::ostream& operator<<(std::ostream &os, const linear_system &ls);

        // Relevant exceptions
        class InfSolutionsMsg : public std::exception {
            virtual const char* what() const throw()
            {
                return "Infinitely many solutions.";
            }
        };
        class NoSolutionMsg : public std::exception {
            virtual const char* what() const throw()
            {
                return "There is no solution.";
            }
        };

    private:
        std::vector<hyperplane> planes;
        size_t dimension;
        void assertion_idx_test(size_t idx);
        bool swap_with_row_below_for_nonzero_coefficient_if_able(size_t lead_idx, size_t current_row_check);
        void clear_coefficients_below(size_t lead_idx, size_t curr_lead_row);
        void clear_coefficients_above(size_t lead_idx, size_t curr_lead_row);
        void compute_parametrization(Result &res, linear_system &rref);
    };
}

#endif //LINEARALGEBRACURSE_LINEAR_SYSTEM_H
