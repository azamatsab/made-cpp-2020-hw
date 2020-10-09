#pragma once

#include <iostream>
#include <vector>

namespace task {

const double EPS = 1e-6;

class OutOfBoundsException : public std::exception {};
class SizeMismatchException : public std::exception {};

class Matrix {
   private:
    double** matrix;

   public:
    size_t rows_;
    size_t cols_;

    Matrix() {
        rows_ = 1;
        cols_ = 1;
        matrix = new double*[rows_];
        matrix[0] = new double[cols_];
        matrix[0][0] = 1.0;
    }

    Matrix(size_t rows, size_t cols) {
        rows_ = rows;
        cols_ = cols;
        matrix = new double*[rows_];
        for (size_t i = 0; i < rows; ++i) {
            matrix[i] = new double[cols_];
            for (size_t j = 0; j < cols; ++j) {
                if (i == j)
                    matrix[i][j] = 1;
                else {
                    matrix[i][j] = 0;
                }
            }
        }
    }

    ~Matrix() {
        for (size_t i = 0; i < rows_; ++i) {
            delete matrix[i];
        }
        delete matrix;
    }

    Matrix(const Matrix& copy) {
        rows_ = copy.rows_;
        cols_ = copy.cols_;
        matrix = new double*[rows_];
        for (size_t i = 0; i < rows_; ++i) {
            matrix[i] = new double[cols_];
            for (size_t j = 0; j < cols_; ++j) {
                matrix[i][j] = copy.matrix[i][j];
            }
        }
    }

    Matrix& operator=(const Matrix& a) {
        if (*this == a) return *this;
        for (size_t i = 0; i < rows_; ++i) {
            delete matrix[i];
        }
        delete matrix;

        rows_ = a.rows_;
        cols_ = a.cols_;
        matrix = new double*[rows_];
        for (size_t i = 0; i < rows_; ++i) {
            matrix[i] = new double[cols_];
            for (size_t j = 0; j < cols_; ++j) {
                matrix[i][j] = a.matrix[i][j];
            }
        }
        return *this;
    }

    double& get(size_t row, size_t col) {
        if (0 > row || row >= rows_ || 0 > col || col >= cols_) {
            throw OutOfBoundsException();
        }
        return matrix[row][col];
    }

    const double& get(size_t row, size_t col) const {
        if (0 > row || row >= rows_ || 0 > col || col >= cols_) {
            throw OutOfBoundsException();
        }
        return matrix[row][col];
    }

    void set(size_t row, size_t col, const double& value) {
        if (0 > row || row >= rows_ || 0 > col || col >= cols_) {
            throw OutOfBoundsException();
        }
        matrix[row][col] = value;
    }

    void resize(size_t new_rows, size_t new_cols) {
        Matrix temp = Matrix(new_rows, new_cols);
        unsigned index;
        unsigned i_hat;
        unsigned j_hat;
        for (size_t i = 0; i < new_rows; ++i) {
            for (size_t j = 0; j < new_cols; ++j) {
                index = i * new_cols + j;
                if (index < rows_ * cols_) {
                    i_hat = index / cols_;
                    j_hat = index % cols_;
                    temp[i][j] = matrix[i_hat][j_hat];
                } else {
                    temp[i][j] = 0.0;
                }
            }
        }
        *this = temp;
    }

    double* operator[](size_t row) { return matrix[row]; }
    const double* operator[](size_t row) const { return matrix[row]; }

    Matrix& operator+=(const Matrix& a) {
        if (a.rows_ != rows_ || a.cols_ != cols_) {
            throw SizeMismatchException();
        }
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < cols_; ++j) {
                matrix[i][j] += a[i][j];
            }
        }
        return *this;
    }
    Matrix& operator-=(const Matrix& a) {
        if (a.rows_ != rows_ || a.cols_ != cols_) {
            throw SizeMismatchException();
        }
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < cols_; ++j) {
                matrix[i][j] -= a[i][j];
            }
        }
        return *this;
    }

    Matrix& operator*=(const Matrix& a) {
        if (a.rows_ != cols_) {
            throw SizeMismatchException();
        }
        Matrix result = Matrix(rows_, a.cols_);
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < a.cols_; ++j) {
                result[i][j] = dotProd(getRow(i), a.getColumn(j));
            }
        }
        *this = result;
        return *this;
    }

    Matrix& operator*=(const double& number) {
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < cols_; ++j) {
                matrix[i][j] *= number;
            }
        }
        return *this;
    }

    Matrix operator+(const Matrix& a) const {
        if (a.rows_ != rows_ || a.cols_ != cols_) {
            throw SizeMismatchException();
        }
        Matrix b = *this;
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < cols_; ++j) {
                b[i][j] += a[i][j];
            }
        }
        return b;
    }

    Matrix operator-(const Matrix& a) const {
        if (a.rows_ != rows_ || a.cols_ != cols_) {
            throw SizeMismatchException();
        }
        Matrix b = *this;
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < cols_; ++j) {
                b[i][j] -= a[i][j];
            }
        }
        return b;
    }

    double dotProd(const std::vector<double>& a,
                   const std::vector<double>& b) const {
        double result = 0;
        for (size_t i = 0; i < a.size(); ++i) {
            result += a[i] * b[i];
        }
        return result;
    }

    Matrix operator*(const Matrix& a) const {
        if (a.rows_ != cols_) {
            throw SizeMismatchException();
        }
        Matrix result = Matrix(rows_, a.cols_);
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < a.cols_; ++j) {
                result[i][j] = dotProd(getRow(i), a.getColumn(j));
            }
        }
        return result;
    }

    Matrix operator*(const double& a) const {
        Matrix b = *this;
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < cols_; ++j) {
                b[i][j] *= a;
            }
        }
        return b;
    }

    Matrix operator-() const {
        Matrix b = *this;
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < cols_; ++j) {
                b[i][j] = -b[i][j];
            }
        }
        return b;
    }

    Matrix operator+() const {
        Matrix b = *this;
        return b;
    }

    double determinant(const Matrix& mat, size_t rows) const {
        if (rows == 1) {
            return mat[0][0];
        } else if (rows == 2) {
            return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
        }
        double result = 0;
        Matrix new_mat = Matrix(rows - 1, rows - 1);
        for (size_t i = 0; i < rows; ++i) {
            size_t t = 0;
            for (size_t j = 0; j < rows; ++j) {
                if (i == j) continue;
                for (size_t k = 1; k < rows; ++k) {
                    new_mat[k - 1][t] = mat[k][j];
                }
                ++t;
            }
            if (i % 2 == 0) {
                result += mat[0][i] * determinant(new_mat, rows - 1);
            } else {
                result -= mat[0][i] * determinant(new_mat, rows - 1);
            }
        }
        return result;
    }

    double det() const {
        if (rows_ != cols_) {
            throw SizeMismatchException();
        }
        return determinant(*this, rows_);
    }
    void transpose() {
        Matrix temp = Matrix(cols_, rows_);
        for (size_t i = 0; i < temp.rows_; ++i) {
            for (size_t j = 0; j < temp.cols_; ++j) {
                temp[i][j] = matrix[j][i];
            }
        }
        *this = temp;
    }

    Matrix transposed() const {
        Matrix b = Matrix(cols_, rows_);
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < cols_; ++j) {
                b.matrix[j][i] = matrix[i][j];
            }
        }
        return b;
    }

    double trace() const {
        if (rows_ != cols_) {
            throw SizeMismatchException();
        }
        double result = 0;
        for (size_t i = 0; i < rows_; ++i) {
            result += matrix[i][i];
        }
        return result;
    }

    std::vector<double> getRow(size_t row) const {
        std::vector<double> result;
        for (size_t j = 0; j < cols_; ++j) {
            result.push_back(matrix[row][j]);
        }
        return result;
    }

    std::vector<double> getColumn(size_t column) const {
        std::vector<double> result;
        for (size_t j = 0; j < rows_; ++j) {
            result.push_back(matrix[j][column]);
        }
        return result;
    }

    std::vector<double> getRow(size_t row) {
        std::vector<double> result;
        for (size_t j = 0; j < cols_; ++j) {
            result.push_back(matrix[row][j]);
        }
        return result;
    }

    std::vector<double> getColumn(size_t column) {
        std::vector<double> result;
        for (size_t j = 0; j < rows_; ++j) {
            result.push_back(matrix[j][column]);
        }
        return result;
    }

    bool operator==(const Matrix& a) const {
        if (a.rows_ != rows_ || a.cols_ != cols_) return false;
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < cols_; ++j) {
                if (std::abs(matrix[i][j] - a[i][j]) > EPS) {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator!=(const Matrix& a) const {
        if (a.rows_ != rows_ || a.cols_ != cols_) return true;
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < cols_; ++j) {
                if (std::abs(matrix[i][j] - a[i][j]) > EPS) {
                    return true;
                }
            }
        }
        return false;
    }
};

Matrix operator*(const double& a, const Matrix& b) {
    Matrix c = Matrix(b.rows_, b.cols_);
    for (size_t i = 0; i < b.rows_; ++i) {
        for (size_t j = 0; j < b.cols_; ++j) {
            c[i][j] = b[i][j] * a;
        }
    }
    return c;
}

std::ostream& operator<<(std::ostream& output, const Matrix& matrix) {
    for (size_t i = 0; i < matrix.rows_; ++i) {
        for (size_t j = 0; j < matrix.cols_; ++j) {
            output << matrix.get(i, j) << " ";
        }
    }
    output << "\n";
    return output;
}
std::istream& operator>>(std::istream& input, Matrix& matrix) {
    size_t row;
    size_t col;
    input >> row;
    input >> col;
    Matrix temp = Matrix(row, col);
    matrix = temp;
    for (size_t i = 0; i < row; ++i) {
        for (size_t j = 0; j < col; ++j) {
            double value;
            input >> value;
            matrix.set(i, j, value);
        }
    }
    return input;
}

}  // namespace task
// namespace task
