#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <vector>

class Matrix
{
public:
    Matrix(size_t row, size_t col);

    Matrix& operator=(Matrix &other);

    Matrix(Matrix && other);

    ~Matrix(){}

    friend Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2);
    friend Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2);
    friend Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t tsize);
    friend bool operator==(Matrix const &mat1, Matrix const &mat2);

    size_t m_row() const { return nrow; }
    size_t m_col() const { return ncol; }
    size_t nrow = 0;
    size_t ncol = 0;
    std::vector <double> data;
    double operator()(size_t row, size_t col) const { return data[row * ncol + col]; }
    double &operator()(size_t row, size_t col) { return data[row * ncol + col]; }

};

Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2);
Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2);
Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t tsize);
bool operator==(Matrix const &mat1, Matrix const &mat2);

#endif