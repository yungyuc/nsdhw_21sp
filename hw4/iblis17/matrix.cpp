#include <algorithm>
#include <atomic>
#include <cstring>
#include <iostream>
#include <memory>
#include <stddef.h>
#include <string>
#include <tuple>
#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


#include <mkl.h>

namespace py = pybind11;


template <class T>
class Allocator42
{
public:
    using value_type      = T;
    using pointer         = T*;
    using const_pointer   = const T*;
    using reference       = T&;
    using const_reference = const T&;
    using size_type       = std::size_t;

    Allocator42()  = default;
    ~Allocator42() = default;

    pointer allocate(size_type n)
    {
        _allocated += n;
        return _allocator.allocate(n);
    }

    void deallocate(pointer p, size_type n) {
        _deallocated += n;
        return _allocator.deallocate(p, n);
    }

    size_type bytes() const       { return (_allocated - _deallocated) * sizeof(T); }
    size_type allocated() const   { return _allocated * sizeof(T); }
    size_type deallocated() const { return _deallocated * sizeof(T); }

    template <class S>
    bool operator== (const Allocator42<S>&) noexcept { return true; }

    template <class S>
    bool operator!= (const Allocator42<S>&) noexcept { return false; }

private:
    static std::allocator<T> _allocator;
    static size_type _allocated;
    static size_type _deallocated;
};

template <class T> std::size_t       Allocator42<T>::_allocated = 0;
template <class T> std::size_t       Allocator42<T>::_deallocated = 0;
template <class T> std::allocator<T> Allocator42<T>::_allocator = std::allocator<T>();

std::size_t bytes()        { return Allocator42<double>().bytes(); }
std::size_t allocated()    { return Allocator42<double>().allocated(); }
std::size_t deallocated()  { return Allocator42<double>().deallocated(); }

class Matrix {

public:
    Matrix(size_t n, size_t m) : _n(n), _m(m), _buf(std::vector<double, Allocator42<double>>(_n * _m, 0))
    {
    }
    Matrix(const Matrix& B) : _n(B._n), _m(B._m), _buf(B._buf) // copy constrcutor
    {
    }
    Matrix(Matrix&& B) : _n(B._n), _m(B._m) // move constrcutor
    {
        _buf = std::move(B._buf);
    }
    Matrix& operator= (Matrix&& B)
    {
        if (this == &B)
            return *this;

        _n   = B._n;
        _m   = B._m;
        _buf = std::move(B._buf);

        return *this;
    }

    double  operator() (const size_t i, const size_t j) const { return this->_buf[i*_m + j]; }
    double& operator() (const size_t i, const size_t j)       { return this->_buf[i*_m + j]; }
    // linear indexing
    double  operator() (const size_t i) const { return this->_buf[i]; }
    double& operator() (const size_t i)       { return this->_buf[i]; }

    size_t n() const { return _n; }
    size_t m() const { return _m; }

    double* data() { return _buf.data(); }

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

        _buf.assign(v.begin(), v.end());
        return this;
    }

    bool eq(Matrix& B) const
    {
        if (_n != B.n())
            return false;
        if (_m != B.m())
            return false;

        const size_t len = _n * _m;
        const double* B_buf = B.data();
        for(size_t i=0; i<len; ++i)
        {
            if (B_buf[i] != _buf[i])
                return false;
        }
        return true;
    }

private:
    size_t _n;
    size_t _m;
    std::vector<double, Allocator42<double>> _buf;
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

Matrix multiply_tile(Matrix& A, Matrix& B, size_t T)
{
    if (A.m() != B.n())
        throw(std::invalid_argument("invalid shape"));

    Matrix C(A.n(), B.m());
    const size_t I = C.n();
    const size_t J = C.m();
    const size_t K = A.m();
    size_t i, j, k, x, y, z;

    for (i=0; i<I; i+=T)
    {
        const size_t X = std::min(i + T, I);
        for (j=0; j<J; j+=T)
        {
            const size_t Y = std::min(j + T, J);
            for (k=0; k<K; k+=T)
            {
                const size_t Z = std::min(k + T, K);
                for (x=i; x<X; ++x)
                    for (y=j; y<Y; ++y)
                    {
                        const size_t xy = x * J + y;
                        for (z=k; z<Z; ++z)
                            C(xy) += A(x * K + z) * B(z * J + y);
                    }
            }
        }
    }

    return C;
}

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
        0.0, // beta
        C.data(),
        n
    );

    return C;
}

PYBIND11_MODULE(_matrix, m) {
    m.def("multiply_naive", &multiply_naive, "");
    m.def("multiply_tile",  &multiply_tile,  "");
    m.def("multiply_mkl",   &multiply_mkl,   "");
    m.def("bytes",          &bytes,          "");
    m.def("allocated",      &allocated,      "");
    m.def("deallocated",    &deallocated,    "");

    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def("n",           &Matrix::n)
        .def("m",           &Matrix::m)
        .def("data",        &Matrix::data)
        .def("from_list",   &Matrix::from_list)
        .def("__repr__",    &Matrix::repr)
        .def("__getitem__", &Matrix::getitem)
        .def("__setitem__", &Matrix::setitem)
        .def("__eq__",      &Matrix::eq)

        .def_property_readonly("nrow", &Matrix::n)
        .def_property_readonly("ncol", &Matrix::m)
        ;
}
