#pragma once

#include <cmath>
#include <iostream>
#include <vector>

namespace task {

const double EPS = 1e-6;

class OutOfBoundsException : public std::exception {};
class SizeMismatchException : public std::exception {};

class Matrix {
 private:
  double** data_;
  size_t row_size;
  size_t col_size;
  double dotProd(const std::vector<double>& a,
                 const std::vector<double>& b) const;
  double determinant(const Matrix& mat, size_t rows) const;
  void checkBounds(size_t row, size_t col) const;
  void checkSize(const Matrix& a) const;
  std::vector<double> getRowVector(size_t row) const;
  std::vector<double> getColumnVector(size_t column) const;
  void setRowSize(size_t size);
  void setColSize(size_t size);

 public:
  Matrix();
  Matrix(size_t rows, size_t cols);
  Matrix(const Matrix& copy);
  Matrix& operator=(const Matrix& a);

  double& get(size_t row, size_t col);
  const double& get(size_t row, size_t col) const;
  void set(size_t row, size_t col, const double& value);
  void resize(size_t new_rows, size_t new_cols);

  double* operator[](size_t row);
  double* operator[](size_t row) const;

  Matrix& operator+=(const Matrix& a);
  Matrix& operator-=(const Matrix& a);
  Matrix& operator*=(const Matrix& a);
  Matrix& operator*=(const double& number);

  Matrix operator+(const Matrix& a) const;
  Matrix operator-(const Matrix& a) const;
  Matrix operator*(const Matrix& a) const;
  Matrix operator*(const double& a) const;

  Matrix operator-() const;
  Matrix operator+() const;

  double det() const;
  void transpose();
  Matrix transposed() const;
  double trace() const;

  std::vector<double> getRow(size_t row);
  std::vector<double> getColumn(size_t column);

  bool operator==(const Matrix& a) const;
  bool operator!=(const Matrix& a) const;

  ~Matrix();
  size_t getRowSize() const;
  size_t getColSize() const;
};

Matrix operator*(const double& a, const Matrix& b);

std::ostream& operator<<(std::ostream& output, const Matrix& matrix);
std::istream& operator>>(std::istream& input, Matrix& matrix);

}  // namespace task
