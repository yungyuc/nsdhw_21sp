#include <mkl.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

#include <new>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

#include "_matrix.hpp"


Matrix::Matrix(int row, int col) : matrix(row * col, 0) {
    _nrow = row;
    _ncol = col;
}

Matrix::Matrix(const Matrix& other)
    : matrix(other._nrow * other._ncol, 0) {
    _nrow = other._nrow;
    _ncol = other._ncol;
    std::copy(other.matrix.begin(), other.matrix.end(), matrix.begin());
}

Matrix::Matrix(const vector<vector<double>>& other) {
    _nrow = other.size();
    _ncol = other[0].size();
    for(const auto &it: other) {
        matrix.insert(matrix.end(), it.begin(), it.end());
    }
}

Matrix::Matrix(Matrix&& other)
    : matrix(std::move(other.matrix)) {
    _nrow = other._nrow;
    _ncol = other._ncol;
}

Matrix multiply_naive(Matrix &a, Matrix &b) {
    Matrix res(a.nrow(), b.ncol());
    for(int i=0;i<res.nrow();i++) {
        for(int j=0;j<res.ncol();j++) {
            double tmp = 0;
            for(int k=0;k<a.ncol();k++) {
                tmp += a(i, k) * b(k, j);
            }
            res(i, j) = tmp;
        }
    }
    return res;
}

Matrix multiply_tile(Matrix &a, Matrix &b, int tile_size) {
    Matrix res(a.nrow(), b.ncol());

    for(int i=0;i<a.nrow();i+=tile_size) {
        int i_min = min(i+tile_size, a.nrow());
        for(int j=0;j<b.ncol();j+=tile_size) {
            int j_min = min(j+tile_size, b.ncol());
            for(int k=0;k<a.ncol();k+=tile_size) {
                int k_min = min(k+tile_size, a.ncol());
                for(int t1=i;t1<i_min;t1++) {
                    for(int t2=j;t2<j_min;t2++) {
                        double tmp = 0;
                        for(int t3=k;t3<k_min;t3++) {
                            tmp += a(t1, t3) * b(t3, t2);
                        }
                        res(t1, t2) += tmp;
                    }
                }
            }
        }
    }

    return res;
}

Matrix multiply_mkl(Matrix &a, Matrix &b) {
    Matrix res(a.nrow(), b.ncol());

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
        a.nrow(), b.ncol(), a.ncol(), 1.0,
        a.data(), a.ncol(),
        b.data(), b.ncol(),
        0,
        res.data(), b.ncol());

    return res;
}
