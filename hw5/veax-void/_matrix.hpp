#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <bits/stdc++.h>

#include "mkl.h"

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

/*PYBIND11_MODULE(_matrix, m) {
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("multiply_tile", &multiply_tile);
    m.def("bytes", &bytes);
    m.def("allocated", &allocated);
    m.def("deallocated", &deallocated);
    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
    .def(py::init<int, int>())
    .def(py::init<vector<vector<double>>&>())
    .def("output", &Matrix::output)
    .def("__eq__", &Matrix::operator==)
    .def("__setitem__", [](Matrix &m, pair<int, int> p, double v) {
        m(p.first, p.second) = v;
    })
    .def("__getitem__", [](Matrix &m, pair<int, int> p) {
        return m(p.first, p.second);
    })
    .def_property("nrow", &Matrix::nrow, nullptr)
    .def_property("ncol", &Matrix::ncol, nullptr)
    .def_buffer([](Matrix &m) -> py::buffer_info {
        return py::buffer_info(
            m.data(),
            sizeof(double),
            py::format_descriptor<double>::format(),
            2,
            { m.nrow(), m.ncol() },
            { sizeof(double) * m.ncol(), sizeof(double) }
        );
    });
}*/