#include <algorithm>
#include <iostream>
#include <stddef.h>


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

    size_t nrow() const { return _n; }
    size_t ncol() const { return _m; }

    Matrix multiply_naive()
    {}

    Matrix multiply_tile()
    {}

    Matrix multiply_mkl()
    {}

private:
    size_t _n;
    size_t _m;
    double* _buf;
};

