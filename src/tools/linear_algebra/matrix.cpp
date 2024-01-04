#include "matrix.hpp"
#include "linear_system.hpp"

namespace LinearAlgebra {


    matrix::matrix(size_t dim) {
        for (size_t i = 0; i < dim; i++) {
            std::vector<double> new_row(dim, 0);
            new_row[i] = 1;
            rows.emplace_back(new_row);
        }
    }

    matrix::matrix(const std::vector<vector> &rows) {
        size_t d = rows[0].dimension();
        for (auto &p : rows) {
            if (p.dimension() != d) throw rowsInDifferentDimException();
        }
        this->rows = rows;
    }

    matrix::matrix(const matrix &mat) {
        rows = mat.rows;
    }

    matrix matrix::operator*(const matrix &mat) const {
        matrix mat_t(mat.t());
        std::vector<vector> new_rows;
        for (size_t i = 0; i < this->size(); i++) {
            std::vector<double> new_row(mat_t.size());
            for (size_t j = 0; j < mat_t.size(); j++) {
                new_row[j] = this->rows[i].dot(mat_t.rows[j]);
            }
            new_rows.emplace_back(new_row);
        }

        return matrix(new_rows);
    }

    matrix matrix::operator*(double scalar) const {
        matrix new_matrix(*this);
        for (auto &row : new_matrix.rows) {
            row = row * scalar;
        }
        return new_matrix;
    }

    matrix matrix::operator/(double scalar) const {
        matrix new_matrix(*this);
        for (auto &row : new_matrix.rows) {
            row = row * (1 / scalar);
        }
        return new_matrix;
    }

    matrix matrix::operator-(const matrix &mat) const {
        matrix new_matrix(*this);
        for (size_t i = 0; i < new_matrix.size(); i++) {
            new_matrix[i] = new_matrix[i] - mat[i];
        }
        return new_matrix;
    }

    matrix matrix::operator+(const matrix &mat) const {
        matrix new_matrix(*this);
        for (size_t i = 0; i < new_matrix.size(); i++) {
            new_matrix[i] = new_matrix[i] + mat[i];
        }
        return new_matrix;
    }

    vector matrix::operator[](size_t row) const {
        if (row > this->size())
            throw rowNotExistsInMatrixException();
        if (row < 0)
            throw illegalRowException();
        return this->rows[row];
    }

    matrix matrix::t() const {
        std::vector<vector> new_rows;

        for (size_t i = 0; i < this->dimension(); i++) {
            std::vector<double> new_row(this->size());
            for (size_t j = 0; j < this->size(); j++) {
                new_row[j] = rows[j][i];
            }
            new_rows.emplace_back(new_row);
        }

        return matrix(new_rows);
    }

    matrix matrix::i() const {
        switch (size()) {
            case 1:
                return matrix({vector({1 / rows[0][0]})});

            case 2:
                if (det() == 0)
                    throw NonInvertibleMatrix();
                return matrix({vector({rows[1][1], -rows[0][1]}),
                               vector({-rows[1][0], rows[0][0]})}) * (1 / det());

            default:
                throw NotImplementedError();
        }
    }

    std::vector<double> matrix::find_eigenvalues() const {
        if (size() != dimension()) {
            throw NonSquareMatrix();
        }
        std::vector<double> eigenvalues;
        switch (size()) {
            case 2: {
                double trace = tr();
                double sqrt = std::sqrt(trace * trace - 4 * det());
                double res1, res2;
                res1 = (trace + sqrt) / 2;
                res2 = (trace - sqrt) / 2;
                eigenvalues.emplace_back(res1);
                /*if (!LinearAlgebra::approximatelyEqualAbsRel(res1, res2)) */
                eigenvalues.emplace_back(res2);
            }
                break;

            case 3: {
                if (is_symmetric()) {
                    double p1 = std::pow(rows[0][1], 2) + std::pow(rows[0][2], 2) + std::pow(rows[1][2], 2);
                    if (!(bool) p1) {
                        // *this is diagonal
                        eigenvalues.emplace_back(rows[0][0]);
                        eigenvalues.emplace_back(rows[1][1]);
                        eigenvalues.emplace_back(rows[2][2]);
                    } else {
                        double q = tr() / 3;
                        double p2 = std::pow(rows[0][0] - q, 2) + std::pow(rows[1][1] - q, 2) +
                                    std::pow(rows[2][2] - q, 2) + 2 * p1;
                        double p = std::sqrt(p2 / 6);
                        matrix I = matrix(size());
                        matrix B = (1 / p) * (*this - q * I);
                        double r = B.det() / 2;

                        // In exact arithmetic for a symmetric matrix  -1 <= r <= 1
                        // but computation error can leave it slightly outside this range.
                        double phi;
                        if (r <= -1) phi = M_PI / 3;
                        else if (r >= 1) phi = 0;
                        else phi = std::acos(r) / 3;

                        eigenvalues = std::vector<double>(3);
                        // the eigenvalues satisfy eig3 <= eig2 <= eig1
                        eigenvalues[0] = q + 2 * p * cos(phi);
                        eigenvalues[2] = q + 2 * p * cos(phi + 2 * M_PI / 3);
                        eigenvalues[1] = 3 * q - eigenvalues[0] - eigenvalues[2];
                    }
                } else {
                    throw NotImplementedError();
                }
            }
                break;

            default:
                throw NotImplementedError();
        }
        return eigenvalues;
    }

    std::vector<vector> matrix::find_eigenvectors() const {
        std::vector<vector> eigenvectors;
        std::vector<double> eigenvalues = find_eigenvalues();

        for (auto &eval : eigenvalues) {
            matrix m(*this);
            m = m.add_value_to_main_diagonal(-eval);
            std::vector<hyperplane> planes;
            for (size_t i = 0; i < m.size(); i++) {
                planes.emplace_back(m.rows[i], 0);
            }
            linear_system ls(planes);
            linear_system::Result ls_res = ls.compute_ge_solution();
            eigenvectors.push_back(ls_res.parametraize[0]);
        }

        return eigenvectors;
    }

    double matrix::tr() const {
        double sum = 0;
        for (size_t i = 0; i < size(); i++) {
            sum += rows[i][i];
        }
        return sum;
    }

    double matrix::det() const {
        if (size() != dimension()) {
            throw NonSquareMatrix();
        }
        return det(*this);
    }

    double matrix::det(const matrix &m) const {
        if (m.size() == 1) return m[0][0];
        double res = 0;
        for (size_t curr_row = 0; curr_row < m.size(); curr_row++) {
            res += (!(curr_row % 2) * 2 - 1) * m[0][curr_row] * det(m.remove_row_and_line(0, curr_row));
        }
        return res;
    }

    matrix matrix::add_value_to_main_diagonal(double val) const {
        matrix res(*this);
        for (size_t i = 0; i < size(); i++) {
            std::vector<double> new_row(res.rows[i].get_orientations());
            new_row[i] += val;
            res.rows[i] = vector(new_row);
        }
        return res;
    }

    size_t matrix::size() const {
        return rows.size();
    }

    size_t matrix::dimension() const {
        return rows[0].dimension();
    }

    std::ostream& operator<<(std::ostream& os, const matrix& mat) {
        os << "{\n";
        for (size_t i = 0; i < mat.size(); i++) {
            os << "\t" << mat.rows[i] << "\n\r";
        }
        os << "}";
        return os;
    }

    matrix matrix::remove_row_and_line(size_t row_number, size_t dimension) const {
        matrix res(*this);
        res.rows.erase(res.rows.begin() + row_number);
        for (auto &row : res.rows) {
            row = row.erase(dimension);
        }
        return res;
    }

    bool matrix::is_symmetric() const {
        if (size() != dimension()) {
            return false;
        }

        bool res = true;

        for (size_t i = 0; i < size() && res; i++) {
            for (size_t j = 0; j < size() && res; j++) {
                if (rows[i][j] != rows[j][i]) {
                    res = false;
                }
            }
        }

        return res;
    }

    matrix operator*(double scalar, const matrix& mat) {
        return mat * scalar;
    }
}