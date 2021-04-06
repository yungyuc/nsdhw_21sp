#include <algorithm>
#include <cstring>
#include <iostream>
#include <stddef.h>
#include <string>
#include <tuple>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <mkl.h>

namespace py = pybind11;


class Matrix {

public:
    Matrix(size_t n, size_t m) : _n(n), _m(m)
    {
        _buf = new double[_n * _m]();
    }
    Matrix(const Matrix& B) : _n(B._n), _m(B._m) // copy constrcutor
    {
        _buf = new double[_n * _m]();
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

    double  operator() (const size_t i, const size_t j) const { return this->_buf[i*_m + j]; }
    double& operator() (const size_t i, const size_t j)       { return this->_buf[i*_m + j]; }

    size_t n() const { return _n; }
    size_t m() const { return _m; }

    double* data() const { return _buf; }

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

    void check_boundary(size_t i, size_t j) const
    {
        if ((i >= _n) || (j >= _m))
            throw(std::out_of_range("out of boundary access"));
    }

    double getitem(std::tuple<size_t, size_t> idx) const
    {
        size_t i = std::get<0>(idx);
        size_t j = std::get<1>(idx);

        check_boundary(i, j);
        return (*this)(i, j);
    }

    void setitem(std::tuple<size_t, size_t> idx, const double val)
    {
        size_t i = std::get<0>(idx);
        size_t j = std::get<1>(idx);

        check_boundary(i, j);
        (*this)(i, j) = val;
    }

    Matrix* from_list(const std::vector<double> &v)
    {
        if (v.size() != _n * _m)
            throw(std::invalid_argument("invalid length"));

        std::memcpy(_buf, v.data(), v.size() * sizeof(double));
        return this;
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
    const size_t I = C.n();
    const size_t J = C.m();
    const size_t K = A.m();

    for(size_t i=0; i<I; ++i)
        for(size_t j=0; j<J; ++j)
            for(size_t k=0; k<K; ++k)
                C(i, j) += A(i, k) * B(k, j);

    return C;
}

Matrix multiply_tile(Matrix& A, Matrix& B)
{}

Matrix multiply_mkl(Matrix& A, Matrix& B)
{
    if (A.m() != B.n())
        throw(std::invalid_argument("invalid shape"));

    Matrix C(A.n(), B.m());
    const size_t m = A.n();
    const size_t n = B.m();
    const size_t k = A.m();

    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        m,
        n,
        k,
        1, // alpha
        A.data(),
        k,
        B.data(),
        n,
        0.0,  // beta
        C.data(),
        n
    );

    return C;
}

PYBIND11_MODULE(_matrix, m) {
    m.def("multiply_naive", &multiply_naive, "");
    m.def("multiply_tile",  &multiply_tile,  "");
    m.def("multiply_mkl",   &multiply_mkl,   "");

    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def("n",           &Matrix::n)
        .def("m",           &Matrix::m)
        .def("data",        &Matrix::data)
        .def("from_list",   &Matrix::from_list)
        .def("__repr__",    &Matrix::repr)
        .def("__getitem__", &Matrix::getitem)
        .def("__setitem__", &Matrix::setitem)

        .def_property_readonly("nrow", &Matrix::n)
        .def_property_readonly("ncol", &Matrix::m)
        ;
}
