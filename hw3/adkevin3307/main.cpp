#include <cassert>
#include <utility>
#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include "mkl.h"

#include "Matrix.h"

using namespace std;

template<typename T>
Matrix<T> multiply_naive(const Matrix<T>& m1, const Matrix<T>& m2)
{
    assert(m1.cols() == m2.rows());

    Matrix<T> m3(m1.rows(), m2.cols());

    size_t rows = m1.rows();
    size_t cols = m2.cols();
    size_t inners = m1.cols();

    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < cols; col++) {
            for (size_t inner = 0; inner < inners; inner++) {
                m3(row, col) += m1(row, inner) * m2(inner, col);
            }
        }
    }

    return m3;
}

template<typename T>
Matrix<T> multiply_tile(const Matrix<T>& m1, const Matrix<T>& m2, size_t size)
{
    assert(m1.cols() == m2.rows());

    Matrix<T> m3(m1.rows(), m2.cols());

    for (size_t row = 0; row < m1.rows(); row += size) {
        for (size_t col = 0; col < m2.cols(); col += size) {
            for (size_t inner = 0; inner < m1.cols(); inner += size) {
                // tile
                for (size_t k = inner; k < min(m1.cols(), inner + size); k++) {
                    for (size_t i = row; i < min(m1.rows(), row + size); i++) {
                        for (size_t j = col; j < min(m2.cols(), col + size); j++) {
                            m3(i, j) += m1(i, k) * m2(k, j);
                        }
                    }
                }
            }
        }
    }

    return m3;
}

template<typename T>
Matrix<T> multiply_mkl(Matrix<T>& m1, Matrix<T>& m2)
{
    assert(m1.cols() == m2.rows());

    Matrix<T> m3(m1.rows(), m2.cols());

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m1.rows(), m2.cols(), m1.cols(), 1, m1.data(), m1.cols(), m2.data(), m2.cols(), 0, m3.data(), m3.cols());

    return m3;
}

PYBIND11_MODULE(_matrix, m)
{
    pybind11::class_<Matrix<double>>(m, "Matrix")
        .def(pybind11::init<size_t, size_t>())
        .def("__getitem__", [](Matrix<double>& m, pair<size_t, size_t> index) {
            return m(index.first, index.second);
        })
        .def("__setitem__", [](Matrix<double>& m, pair<size_t, size_t> index, double value) {
            m(index.first, index.second) = value;
        })
        .def("__str__", [](Matrix<double>& m) {
            stringstream ss;
            ss << m;

            return ss.str();
        })
        .def(pybind11::self == pybind11::self)
        .def_property_readonly("nrow", &Matrix<double>::rows)
        .def_property_readonly("ncol", &Matrix<double>::cols);

    m.def("multiply_naive", multiply_naive<double>, "Matrix multiply with naive method.");
    m.def("multiply_tile", multiply_tile<double>, "Matrix multiply with tile method.");
    m.def("multiply_mkl", multiply_mkl<double>, "Matrix multiply with mkl method.");
}
