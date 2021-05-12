#pragma once

#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

#include <mkl.h>
#include <vector>

using namespace std;
namespace py = pybind11;

class Matrix {
public:
    Matrix() {}
    Matrix(int row, int col);
    Matrix(const Matrix& other);
    Matrix(const vector<vector<double>>& other);
    Matrix(Matrix&& other);
    ~Matrix() {
        matrix.reserve(0);
    }
    int nrow() const { return _nrow; }
    int ncol() const { return _ncol; }
    double *data() { return matrix.data(); }
    double operator() (int row, int col) const { return matrix[row*_ncol + col]; }
    double &operator() (int row, int col) { return matrix[row*_ncol + col]; }

    bool operator == (const Matrix& other) {
        if(this == &other) return true;
        if(_nrow != other._nrow || _ncol != other._ncol) return false;
        for(int i=0;i<_nrow;i++) {
            for(int j=0;j<_ncol;j++) {
                if(other(i, j) != (*this)(i, j)) {
                    return false;
                }
            }
        }
        return true;
    }

    friend bool operator == (const Matrix& mat1, const Matrix& mat2);

    void output() {
        for(int i=0;i<_nrow;i++) {
            for(int j=0;j<_ncol;j++) {
                cout<<setprecision(10)<<matrix[i*_ncol+j]<<" ";
            }
            cout<<"\n";
        }
    }


private:
    vector<double> matrix;
    int _nrow, _ncol;
};

bool operator == (const Matrix& mat1, const Matrix& mat2) {
    if(&mat1 == &mat2) return true;
    if(mat1._nrow != mat2._nrow || mat1._ncol != mat2._ncol) return false;
    for(int i=0;i<mat1._nrow;i++) {
        for(int j=0;j<mat1._ncol;j++) {
            if(mat1(i, j) != mat2(i, j)) {
                return false;
            }
        }
    }
    return true;
}

class Tiller : Matrix {
public:
    Tiller(int n) : res(n, n), mat_a(n, n), mat_b(n, n), s(n) {}
    void load(Matrix &a, int row1, int col1, Matrix &b, int row2, int col2) {
        for(int i=0;i<s;i++) {
            for(int j=0;j<s;j++) {
                mat_a(i, j) = a(row1+i, col1+j);
                mat_b(i, j) = b(row2+i, col2+j);
            }
        }
    }
    void multiply() {
        for(int i=0;i<s;i++) {
            for(int j=0;j<s;j++) {
                double tmp = 0;
                for(int k=0;k<s;k++) {
                    tmp += mat_a(i, k) * mat_b(k, j);
                }
                res(i, j) = tmp;
            }
        }
    }
    Matrix res;
    Matrix mat_a;
    Matrix mat_b;
    int s;
};
