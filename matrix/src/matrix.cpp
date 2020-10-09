#include "matrix.h"

using namespace task;

Matrix::Matrix() {
  setRowSize(1);
  setColSize(1);
  data_ = new double*[getRowSize()];
  data_[0] = new double[getColSize()];
  data_[0][0] = 1.0;
}

Matrix::Matrix(size_t rows, size_t cols) {
  setRowSize(rows);
  setColSize(cols);
  data_ = new double*[rows];
  for (size_t i = 0; i < rows; ++i) {
    data_[i] = new double[cols];
    for (size_t j = 0; j < cols; ++j) {
      if (i == j)
        data_[i][j] = 1;
      else {
        data_[i][j] = 0;
      }
    }
  }
}

Matrix::~Matrix() {
  for (size_t i = 0; i < getRowSize(); ++i) {
    delete data_[i];
  }
  delete data_;
}

Matrix::Matrix(const Matrix& copy) {
  setRowSize(copy.getRowSize());
  setColSize(copy.getColSize());
  data_ = new double*[getRowSize()];
  for (size_t i = 0; i < getRowSize(); ++i) {
    data_[i] = new double[getColSize()];
    for (size_t j = 0; j < getColSize(); ++j) {
      data_[i][j] = copy[i][j];
    }
  }
}

Matrix& Matrix::operator=(const Matrix& a) {
  if (*this == a) return *this;
  for (size_t i = 0; i < getRowSize(); ++i) {
    delete data_[i];
  }
  delete data_;

  setRowSize(a.getRowSize());
  setColSize(a.getColSize());
  data_ = new double*[getRowSize()];
  for (size_t i = 0; i < getRowSize(); ++i) {
    data_[i] = new double[getColSize()];
    for (size_t j = 0; j < getColSize(); ++j) {
      data_[i][j] = a[i][j];
    }
  }
  return *this;
}

void Matrix::checkBounds(size_t row, size_t col) const {
  if (0 > row || row >= getRowSize() || 0 > col || col >= getColSize()) {
    throw OutOfBoundsException();
  }
}

double& Matrix::get(size_t row, size_t col) {
  checkBounds(row, col);
  return data_[row][col];
}

const double& Matrix::get(size_t row, size_t col) const {
  checkBounds(row, col);
  return data_[row][col];
}

void Matrix::set(size_t row, size_t col, const double& value) {
  checkBounds(row, col);
  data_[row][col] = value;
}

void Matrix::resize(size_t new_rows, size_t new_cols) {
  Matrix temp = Matrix(new_rows, new_cols);
  unsigned index;
  unsigned i_hat;
  unsigned j_hat;
  int size = getRowSize() * getColSize();
  for (size_t i = 0; i < new_rows; ++i) {
    for (size_t j = 0; j < new_cols; ++j) {
      index = i * new_cols + j;
      if (index < size) {
        i_hat = index / getColSize();
        j_hat = index % getColSize();
        temp[i][j] = data_[i_hat][j_hat];
      } else {
        temp[i][j] = 0.0;
      }
    }
  }
  *this = temp;
}

double* Matrix::operator[](size_t row) { return data_[row]; }

double* Matrix::operator[](size_t row) const { return data_[row]; }

void Matrix::checkSize(const Matrix& a) const {
  if (a.getRowSize() != getRowSize() || a.getColSize() != getColSize()) {
    throw SizeMismatchException();
  }
}

Matrix& Matrix::operator+=(const Matrix& a) {
  checkSize(a);
  for (size_t i = 0; i < getRowSize(); ++i) {
    for (size_t j = 0; j < getColSize(); ++j) {
      data_[i][j] += a[i][j];
    }
  }
  return *this;
}

Matrix& Matrix::operator-=(const Matrix& a) {
  checkSize(a);
  for (size_t i = 0; i < getRowSize(); ++i) {
    for (size_t j = 0; j < getColSize(); ++j) {
      data_[i][j] -= a[i][j];
    }
  }
  return *this;
}

Matrix& Matrix::operator*=(const Matrix& a) {
  if (a.getRowSize() != getColSize()) {
    throw SizeMismatchException();
  }
  Matrix result = Matrix(getRowSize(), a.getColSize());
  for (size_t i = 0; i < getRowSize(); ++i) {
    for (size_t j = 0; j < a.getColSize(); ++j) {
      result[i][j] = dotProd(getRowVector(i), a.getColumnVector(j));
    }
  }
  *this = result;
  return *this;
}

Matrix& Matrix::operator*=(const double& number) {
  for (size_t i = 0; i < getRowSize(); ++i) {
    for (size_t j = 0; j < getColSize(); ++j) {
      data_[i][j] *= number;
    }
  }
  return *this;
}

Matrix Matrix::operator+(const Matrix& a) const {
  checkSize(a);
  Matrix b = *this;
  for (size_t i = 0; i < getRowSize(); ++i) {
    for (size_t j = 0; j < getColSize(); ++j) {
      b[i][j] += a[i][j];
    }
  }
  return b;
}

Matrix Matrix::operator-(const Matrix& a) const {
  checkSize(a);
  Matrix b = *this;
  for (size_t i = 0; i < getRowSize(); ++i) {
    for (size_t j = 0; j < getColSize(); ++j) {
      b[i][j] -= a[i][j];
    }
  }
  return b;
}

double Matrix::dotProd(const std::vector<double>& a,
                       const std::vector<double>& b) const {
  double result = 0;
  for (size_t i = 0; i < a.size(); ++i) {
    result += a[i] * b[i];
  }
  return result;
}

Matrix Matrix::operator*(const Matrix& a) const {
  if (a.getRowSize() != getColSize()) {
    throw SizeMismatchException();
  }
  Matrix result = Matrix(getRowSize(), a.getColSize());
  for (size_t i = 0; i < getRowSize(); ++i) {
    for (size_t j = 0; j < a.getColSize(); ++j) {
      result[i][j] = dotProd(getRowVector(i), a.getColumnVector(j));
    }
  }
  return result;
}

Matrix Matrix::operator*(const double& a) const {
  Matrix b = *this;
  for (size_t i = 0; i < getRowSize(); ++i) {
    for (size_t j = 0; j < getColSize(); ++j) {
      b[i][j] *= a;
    }
  }
  return b;
}

Matrix Matrix::operator-() const {
  Matrix b = *this;
  for (size_t i = 0; i < getRowSize(); ++i) {
    for (size_t j = 0; j < getColSize(); ++j) {
      b[i][j] = -b[i][j];
    }
  }
  return b;
}

Matrix Matrix::operator+() const {
  Matrix b = *this;
  return b;
}

double Matrix::determinant(const Matrix& mat, size_t rows) const {
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

double Matrix::det() const {
  if (getRowSize() != getColSize()) {
    throw SizeMismatchException();
  }
  return determinant(*this, getRowSize());
}

void Matrix::transpose() {
  Matrix temp = Matrix(getColSize(), getRowSize());
  for (size_t i = 0; i < temp.getRowSize(); ++i) {
    for (size_t j = 0; j < temp.getColSize(); ++j) {
      temp[i][j] = data_[j][i];
    }
  }
  *this = temp;
}

Matrix Matrix::transposed() const {
  Matrix b = Matrix(getColSize(), getRowSize());
  for (size_t i = 0; i < getRowSize(); ++i) {
    for (size_t j = 0; j < getColSize(); ++j) {
      b[j][i] = data_[i][j];
    }
  }
  return b;
}

double Matrix::trace() const {
  if (getRowSize() != getColSize()) {
    throw SizeMismatchException();
  }
  double result = 0;
  for (size_t i = 0; i < getRowSize(); ++i) {
    result += data_[i][i];
  }
  return result;
}

std::vector<double> Matrix::getRowVector(size_t row) const {
  std::vector<double> result(getColSize());
  for (size_t j = 0; j < getColSize(); ++j) {
    result[j] = data_[row][j];
  }
  return result;
}

std::vector<double> Matrix::getColumnVector(size_t column) const {
  std::vector<double> result(getRowSize());
  for (size_t j = 0; j < getRowSize(); ++j) {
    result[j] = data_[j][column];
  }
  return result;
}

std::vector<double> Matrix::getRow(size_t row) { return getRowVector(row); }

std::vector<double> Matrix::getColumn(size_t column) {
  return getColumnVector(column);
}

bool Matrix::operator==(const Matrix& a) const {
  if (a.getRowSize() != getRowSize() || a.getColSize() != getColSize())
    return false;
  for (size_t i = 0; i < getRowSize(); ++i) {
    for (size_t j = 0; j < getColSize(); ++j) {
      if (fabs(data_[i][j] - a[i][j]) > EPS) {
        return false;
      }
    }
  }
  return true;
}

bool Matrix::operator!=(const Matrix& a) const {
  if (a.getRowSize() != getRowSize() || a.getColSize() != getColSize())
    return true;
  for (size_t i = 0; i < getRowSize(); ++i) {
    for (size_t j = 0; j < getColSize(); ++j) {
      if (fabs(data_[i][j] - a[i][j]) > EPS) {
        return true;
      }
    }
  }
  return false;
}

size_t Matrix::getRowSize() const { return row_size; }

size_t Matrix::getColSize() const { return col_size; }

void Matrix::setRowSize(size_t size) { row_size = size; }

void Matrix::setColSize(size_t size) { col_size = size; }

Matrix task::operator*(const double& a, const Matrix& b) {
  Matrix c = Matrix(b.getRowSize(), b.getColSize());
  for (size_t i = 0; i < b.getRowSize(); ++i) {
    for (size_t j = 0; j < b.getColSize(); ++j) {
      c[i][j] = b[i][j] * a;
    }
  }
  return c;
}

std::ostream& task::operator<<(std::ostream& output, const Matrix& matrix) {
  for (size_t i = 0; i < matrix.getRowSize(); ++i) {
    for (size_t j = 0; j < matrix.getColSize(); ++j) {
      output << matrix.get(i, j) << " ";
    }
  }
  output << "\n";
  return output;
}

std::istream& task::operator>>(std::istream& input, Matrix& matrix) {
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
