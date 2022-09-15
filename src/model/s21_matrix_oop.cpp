#include "s21_matrix_oop.h"

#include <random>

S21Matrix::S21Matrix() : _rows(4), _columns(4), _matrix(nullptr) {
  _matrix = S21Matrix::allocateMemory(_rows, _columns);
}

S21Matrix::S21Matrix(int rows, int columns)
    : _rows(rows), _columns(columns), _matrix(nullptr) {
  if (rows <= 0 || columns <= 0) {
    throw S21MatrixExcepcion(
        "Incorrect matrix size. Rows and columns must be more, than 0");
  }

  _matrix = S21Matrix::allocateMemory(rows, columns);
}

S21Matrix::S21Matrix(const S21Matrix& object)
    : _rows(object._rows), _columns(object._columns), _matrix(nullptr) {
  if (object._rows <= 0 || object._columns <= 0) {
    throw S21MatrixExcepcion(
        "Incorrect matrix size. Rows and columns must be more, than 0");
  }

  _matrix = S21Matrix::allocateMemory(object._rows, object._columns);
  S21Matrix::fillMatrix(_matrix, object._matrix, object._rows, object._columns);
}

S21Matrix::S21Matrix(S21Matrix&& object)
    : _rows(0), _columns(0), _matrix(nullptr) {
  if (object._rows <= 0 || object._columns <= 0) {
    throw S21MatrixExcepcion(
        "Incorrect matrix size. Rows and columns must be more, than 0");
  }

  std::swap(_matrix, object._matrix);
  std::swap(_rows, object._rows);
  std::swap(_columns, object._columns);
}

S21Matrix::~S21Matrix() {
  if (_matrix) {
    freeMemory(_matrix, _rows);
    _rows = 0;
    _columns = 0;
  }
}

int S21Matrix::getColumns() { return _columns; }

int S21Matrix::getRows() { return _rows; }

void S21Matrix::setColumns(int columns) {
  if (columns <= 0) {
    throw S21MatrixExcepcion("Columns size must be more, than 0");
  }

  if (columns != _columns) {
    double** tempMatrix = allocateMemory(_rows, _columns);
    fillMatrix(tempMatrix, _matrix, _rows, _columns);
    freeMemory(_matrix, _rows);
    _matrix = allocateMemory(_rows, columns);

    if (columns > _columns) {
      fillMatrix(_matrix, tempMatrix, _rows, _columns);
    } else {
      fillMatrix(_matrix, tempMatrix, _rows, columns);
    }

    freeMemory(tempMatrix, _rows);
    _columns = columns;
  }
}

void S21Matrix::setRows(int rows) {
  if (rows <= 0) {
    throw S21MatrixExcepcion("Rows size must be more, than 0");
  }

  if (rows != _rows) {
    double** tempMatrix = allocateMemory(_rows, _columns);
    fillMatrix(tempMatrix, _matrix, _rows, _columns);
    freeMemory(_matrix, _rows);

    _matrix = allocateMemory(rows, _columns);

    if (rows > _rows) {
      fillMatrix(_matrix, tempMatrix, _rows, _columns);
    } else {
      fillMatrix(_matrix, tempMatrix, rows, _columns);
    }

    freeMemory(tempMatrix, _rows);
    _rows = rows;
  }
}

bool S21Matrix::eq_matrix(const S21Matrix& object) {
  bool result = true;

  if (_rows != object._rows || _columns != object._columns) {
    result = false;
  } else {
    for (int i = 0; i < _rows; i++) {
      for (int j = 0; j < _columns; j++) {
        if (fabs(_matrix[i][j] - object._matrix[i][j]) >= EPSILON) {
          result = false;
        }
      }
    }
  }

  return result;
}

void S21Matrix::sum_matrix(const S21Matrix& object) {
  if (_rows != object._rows || _columns != object._columns) {
    throw S21MatrixExcepcion("Different dimensions of matrices");
  }

  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _columns; j++) {
      _matrix[i][j] += object._matrix[i][j];
    }
  }
}

void S21Matrix::sub_matrix(const S21Matrix& object) {
  if (_rows != object._rows || _columns != object._columns) {
    throw S21MatrixExcepcion("Different dimensions of matrices");
  }

  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _columns; j++) {
      _matrix[i][j] -= object._matrix[i][j];
    }
  }
}

void S21Matrix::mul_number(const double num) {
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _columns; j++) {
      _matrix[i][j] *= num;
    }
  }
}

void S21Matrix::mul_matrix(const S21Matrix& object) {
  if (_columns != object._rows) {
    throw S21MatrixExcepcion(
        "Number of columns first is not equal to number of rows second matrix");
  }

  S21Matrix resultMatrix = S21Matrix(_rows, object._columns);

  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < object._columns; j++) {
      for (int k = 0; k < object._rows; k++) {
        resultMatrix._matrix[i][j] += _matrix[i][k] * object._matrix[k][j];
      }
    }
  }

  *this = resultMatrix;
}

S21Matrix S21Matrix::multWithSigmoid(const S21Matrix& object) {
  if (_columns != object._rows) {
    throw S21MatrixExcepcion(
        "Number of columns first is not equal to number of rows second matrix");
  }

  S21Matrix resultMatrix = S21Matrix(_rows, object._columns);

  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < object._columns; j++) {
      for (int k = 0; k < object._rows; k++) {
        resultMatrix(i, j) += _matrix[i][k] * object._matrix[k][j];
      }

      resultMatrix(i, j) = 1 / (1 + exp(-resultMatrix(i, j)));
      // std::cout << resultMatrix(i, j) << std::endl;
    }
  }

  return resultMatrix;
}

S21Matrix S21Matrix::transpose() {
  S21Matrix resultMatrix = S21Matrix(_columns, _rows);

  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _columns; j++) {
      resultMatrix._matrix[j][i] = _matrix[i][j];
    }
  }

  return resultMatrix;
}

double S21Matrix::determinant() {
  if (!(this->isSquareMatrix())) {
    throw S21MatrixExcepcion("Matrix is not square");
  }

  double determinant = 0;

  if (_rows == 1) {
    determinant = _matrix[0][0];
  } else if (_rows == 2) {
    determinant =
        (_matrix[0][0] * _matrix[1][1]) - (_matrix[0][1] * _matrix[1][0]);
  } else {
    for (int i = 0; i < _rows; i++) {
      S21Matrix minor = this->cutMatrix(0, i);
      determinant += _matrix[0][i] * pow(-1, i) * minor.determinant();
    }
  }

  return determinant;
}

S21Matrix S21Matrix::cutMatrix(int a, int b) {
  S21Matrix resultMatrix = S21Matrix(_rows - 1, _columns - 1);
  int new_rows = 0;
  int new_columns = 0;

  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _columns; j++) {
      if (i != a && j != b) {
        resultMatrix._matrix[new_rows][new_columns] = this->_matrix[i][j];
        new_columns++;

        if (new_columns == _rows - 1) {
          new_columns = 0;
          new_rows++;
        }
      }
    }
  }

  return resultMatrix;
}

S21Matrix S21Matrix::calc_complements() {
  S21Matrix resultMatrix = S21Matrix(_rows, _columns);

  if (!(this->isSquareMatrix())) {
    throw S21MatrixExcepcion("matrix is not square");
  }

  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _columns; j++) {
      S21Matrix matrix = this->cutMatrix(j, i);
      resultMatrix._matrix[j][i] = pow(-1, i + j) * matrix.determinant();
    }
  }

  return resultMatrix;
}

S21Matrix S21Matrix::inverse_matrix() {
  if (this->determinant() == 0) {
    throw S21MatrixExcepcion("Matrix determinant is 0");
  }

  S21Matrix tempMatrix = this->calc_complements();
  S21Matrix resultMatrix = tempMatrix.transpose();
  resultMatrix.mul_number(1 / this->determinant());

  return resultMatrix;
}

S21Matrix S21Matrix::operator+(const S21Matrix& object) {
  if (object._rows != _rows || object._columns != _columns) {
    throw S21MatrixExcepcion("Different dimensions of matrices");
  }

  S21Matrix resultMatrix(*this);
  resultMatrix.sum_matrix(object);
  return resultMatrix;
}

S21Matrix& S21Matrix::operator+=(const S21Matrix& object) {
  if (object._rows != _rows || object._columns != _columns) {
    throw S21MatrixExcepcion("Different dimensions of matrices");
  }

  this->sum_matrix(object);

  return *this;
}

S21Matrix S21Matrix::operator-(const S21Matrix& object) {
  if (object._rows != _rows || object._columns != _columns) {
    throw S21MatrixExcepcion("Different dimensions of matrices");
  }

  S21Matrix resultMatrix(*this);
  resultMatrix.sub_matrix(object);
  return resultMatrix;
}

S21Matrix& S21Matrix::operator-=(const S21Matrix& object) {
  if (object._rows != _rows || object._columns != _columns) {
    throw S21MatrixExcepcion("Different dimensions of matrices");
  }

  this->sub_matrix(object);

  return *this;
}

S21Matrix S21Matrix::operator*(const S21Matrix& object) {
  if (_columns != object._rows) {
    throw S21MatrixExcepcion(
        "Number of columns first is not equal to number of rows second matrix");
  }

  S21Matrix resultMatrix(*this);
  resultMatrix.mul_matrix(object);

  return resultMatrix;
}

S21Matrix& S21Matrix::operator*=(const S21Matrix& object) {
  if (_columns != object._rows) {
    throw S21MatrixExcepcion(
        "Number of columns first is not equal to number of rows second matrix");
  }

  this->mul_matrix(object);

  return *this;
}

S21Matrix S21Matrix::operator*(const double num) {
  S21Matrix resultMatrix(*this);
  resultMatrix.mul_number(num);

  return resultMatrix;
}

S21Matrix& S21Matrix::operator*=(const double num) {
  this->mul_number(num);

  return *this;
}

bool S21Matrix::operator==(const S21Matrix& object) {
  return this->eq_matrix(object);
}

S21Matrix& S21Matrix::operator=(const S21Matrix& object) {
  if (this != &object) {
    _matrix = freeMemory(_matrix, object._rows);
    _matrix = allocateMemory(object._rows, object._columns);
    fillMatrix(_matrix, object._matrix, object._rows, object._columns);
    _rows = object._rows;
    _columns = object._columns;
  }

  return *this;
}

double& S21Matrix::operator()(int row, int column) {
  if (row > _rows || row < 0 || column > _columns || column < 0) {
    throw S21MatrixExcepcion("Matrix index is out of range");
  }

  return _matrix[row][column];
}

void S21Matrix::fillMatrix(double** destMatrix, double** srcMatrix, int rows,
                           int columns) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      destMatrix[i][j] = srcMatrix[i][j];
    }
  }
}

void S21Matrix::fillMatrixRandomValue() {
  std::random_device rd;
  std::default_random_engine eng(rd());
  std::uniform_real_distribution<double> distr(-1, 1);

  for (int i = 0; i < getRows(); i++) {
    for (int j = 0; j < getColumns(); j++) {
      this->operator()(i, j) = distr(eng);
    }
  }
}

void S21Matrix::fillMatrixFromVector(const std::vector<double> data) {
  int vectorCounter = 0;

  for (int i = 0; i < getRows(); i++) {
    for (int j = 0; j < getColumns(); j++) {
      this->operator()(i, j) = data.at(vectorCounter);
      vectorCounter++;
    }
  }
}

double** S21Matrix::allocateMemory(const int rows, const int columns) {
  double** data = new double*[rows];

  for (int i = 0; i < rows; i++) {
    data[i] = new double[columns]();
  }

  return data;
}

double** S21Matrix::freeMemory(double** data, const int rows) {
  for (int i = 0; i < rows; i++) {
    delete[] data[i];
  }

  delete[] data;

  return data;
}

bool S21Matrix::isSquareMatrix() {
  if (_columns != _rows) {
    return false;
  }

  return true;
}
