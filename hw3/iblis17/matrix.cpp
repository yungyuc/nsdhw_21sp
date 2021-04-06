#include <algorithm>
#include <iostream>
#include <stddef.h>
#include <string>

#include <pybind11/pybind11.h>

namespace py = pybind11;


class Matrix {

public:
    Matrix(size_t n, size_t m) : _n(n), _m(m)
    {
        _buf = new double[_n * _m];
    }
    Matrix(const Matrix& B) : _n(B._n), _m(B._m) // copy constrcutor
    {
        _buf = new double[_n * _m];
        std::copy(B._buf, B._buf + B._n * B._m, _buf);
    }
    Matrix(Matrix&& B) : _n(B._n), _m(B._m), _buf(B._buf) // move constrcutor
    {
        std::cout << "move constructor called" << std::endl;
        B._buf = nullptr; // prevent double-free
    }
    Matrix& operator= (Matrix&& B)
    {
        if (this == &B)
            return *this;

        delete[] _buf;

        _n   = B._n;
        _m   = B._m;
        _buf = B._buf;

        B._buf = nullptr;

        return *this;
    }

    ~Matrix() { delete[] _buf; }

    double  operator() (size_t i, size_t j) const { return this->_buf[i*_m + j]; }
    double& operator() (size_t i, size_t j)       { return this->_buf[i*_m + j]; }

    size_t n() const { return _n; }
    size_t m() const { return _m; }

    double* val() const { return _buf; }

    std::string repr() const {
        std::string s = std::to_string(_n) + 'x' + std::to_string(_m) + '\n';

        for (size_t i=0; i<_n; ++i)
        {
            for (size_t j=0; j<_m; ++j)
                s += std::to_string(_buf[i*_m + j]) + ' ';
            s += '\n';
        }

        return s;
    }

private:
    size_t _n;
    size_t _m;
    double* _buf;
};


Matrix multiply_naive(Matrix& A, Matrix& B)
{
    if (A.m() != B.n())
        throw(std::invalid_argument("invalid shape"));

    Matrix C(A.n(), B.m());

    for(size_t i=0; i<C.n(); ++i)
        for(size_t j=0; j<C.m(); ++j)
            for(size_t k=0; k<A.m(); ++k)
                C(i, j) += A(i, k) * B(k, j);

    return C;
}

Matrix multiply_tile(Matrix& A, Matrix& B)
{}

Matrix multiply_mkl(Matrix& A, Matrix& B)
{}

PYBIND11_MODULE(_matrix, m) {
    m.def("multiply_naive", &multiply_naive, "");
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def("n", &Matrix::n)
        .def("m", &Matrix::m)
        .def("val", &Matrix::val)
        .def("__repr__", &Matrix::repr)
        ;
}
