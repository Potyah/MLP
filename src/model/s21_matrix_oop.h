#ifndef SRC_MODEL_S21_MATRIX_OOP_H_
#define SRC_MODEL_S21_MATRIX_OOP_H_

#include <math.h>

#include <exception>
#include <iostream>
#include <vector>
#define EPSILON 1e-7

class S21MatrixExcepcion : public std::exception {
private:
    std::string _errorMessage;

public:
    explicit S21MatrixExcepcion(const std::string& error) : _errorMessage(error) {}
    ~S21MatrixExcepcion() throw() {}
    const char* what() const throw() { return _errorMessage.c_str(); }
};

class S21Matrix {
private:
    int _rows, _columns;
    double** _matrix;

    S21Matrix cutMatrix(int a, int b);
    void fillMatrix(double** destMatrix, double** srcMatrix, int rows, int columns);
    double** allocateMemory(const int rows, const int columns);
    double** freeMemory(double** data, const int rows);
    bool isSquareMatrix();

public:
    S21Matrix();
    S21Matrix(int rows, int columns);
    S21Matrix(const S21Matrix& object);
    S21Matrix(S21Matrix&& object);
    ~S21Matrix();

    int getColumns();
    int getRows();
    void setColumns(int columns);
    void setRows(int rows);

    bool eq_matrix(const S21Matrix& object);
    void sum_matrix(const S21Matrix& object);
    void sub_matrix(const S21Matrix& object);
    void mul_number(const double num);
    void mul_matrix(const S21Matrix& object);
    S21Matrix multWithSigmoid(const S21Matrix& object);
    S21Matrix transpose();
    S21Matrix calc_complements();
    double determinant();
    S21Matrix inverse_matrix();

    S21Matrix operator+(const S21Matrix& object);
    S21Matrix& operator+=(const S21Matrix& object);
    S21Matrix operator-(const S21Matrix& object);
    S21Matrix& operator-=(const S21Matrix& object);
    S21Matrix operator*(const S21Matrix& object);
    S21Matrix& operator*=(const S21Matrix& object);
    S21Matrix operator*(const double num);
    S21Matrix& operator*=(const double num);
    bool operator==(const S21Matrix& object);
    S21Matrix& operator=(const S21Matrix& object);
    double& operator()(int row, int column);

    void fillMatrixRandomValue();
    void fillMatrixFromVector(const std::vector<double> data);
};
#endif  //  SRC_MODEL_S21_MATRIX_OOP_H_
