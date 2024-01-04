#ifndef LINEARALGEBRACURSE_MATRIX_H
#define LINEARALGEBRACURSE_MATRIX_H

#include "utilities/linear_algebra.hpp"
#include "vector.hpp"

namespace LinearAlgebra {

    class matrix {
    public:
        matrix(size_t dim);
        matrix(const std::vector<vector> &mat);
        matrix(const matrix &mat);
        matrix operator*(const matrix&) const;
        matrix operator*(double scalar) const;
        friend matrix operator*(double scalar, const matrix& mat);
        matrix operator/(double scalar) const;
        friend matrix operator/(double scalar, const matrix& mat);
        matrix operator-(const matrix&) const;
        matrix operator+(const matrix&) const;

        /**
         *
         * @param row - row number
         * @return vector object of the specified row number
         *
         * @throw <b>rowNotExistsInMatrixException</b> - if the row does not exists
         * @throw <b>illegalRowException</b> - if specified row is lower then 0
         */
        vector operator[](size_t row) const;

        /**
         * @brief Get transpose matrix
         * @return The transpose matrix
         */
        matrix t() const;

        /**
         * @brief Get inverse matrix
         * @return The inverse matrix
         *
         * @throw <b>NotImplementedError</b> - This function currently implements only the inverse matrix of dimensions 1 & 2.
         * In higher dimensions, this exception will be thrown.
         */
        matrix i() const;

        double tr() const;

        bool is_symmetric() const;

        double det() const;

        matrix add_value_to_main_diagonal(double val) const;

        std::vector<double> find_eigenvalues() const;
        std::vector<vector> find_eigenvectors() const;
        size_t size() const;
        size_t dimension() const;
        friend std::ostream& operator<<(std::ostream& os, const matrix& mat);

        class NonInvertibleMatrix : public std::exception {
            virtual const char* what() const throw()
            {
                return "This matrix is a non-invertible matrix.";
            }
        };

        class InvertibleMatrix : public std::exception {
            virtual const char* what() const throw()
            {
                return "This matrix is an invertible matrix.";
            }
        };

        class NonSquareMatrix : public std::exception {
            virtual const char* what() const throw()
            {
                return "This operation requires the matrix to be square - and this matrix violate this condition.";
            }
        };

    private:
        std::vector<vector> rows;

        matrix remove_row_and_line(size_t row_number, size_t dimension) const;
        double det(const matrix &m) const;
    };
}


#endif //LINEARALGEBRACURSE_MATRIX_H
