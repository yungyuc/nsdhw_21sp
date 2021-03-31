#include <cassert>
#include <utility>
#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

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

    size_t tile_rows = m1.rows() / size + (m1.rows() % size == 0 ? 0 : 1);
    size_t tile_cols = m2.cols() / size + (m2.cols() % size == 0 ? 0 : 1);
    size_t tile_inners = m1.cols() / size + (m1.cols() % size == 0 ? 0 : 1);

    Matrix<T> tile_m1(size, size);
    Matrix<T> tile_m2(size, size);

    for (size_t row = 0; row < tile_rows; row++) {
        for (size_t col = 0; col < tile_cols; col++) {
            for (size_t inner = 0; inner < tile_inners; inner++) {
                // copy
                for (size_t i = 0; i < size; i++) {
                    for (size_t j = 0; j < size; j++) {
                        size_t index_row = row * size + i;
                        size_t index_col = col * size + i;
                        size_t index_inner = inner * size + j;

                        if (index_row < m1.rows() && index_inner < m1.cols()) {
                            tile_m1(i, j) = m1(index_row, index_inner);
                        }
                        else {
                            tile_m1(i, j) = 0;
                        }

                        if (index_inner < m2.rows() && index_col < m2.cols()) {
                            tile_m2(i, j) = m2(index_inner, index_col);
                        }
                        else {
                            tile_m2(i, j) = 0;
                        }
                    }
                }
                // calculate
                for (size_t i = 0; i < size; i++) {
                    for (size_t j = 0; j < size; j++) {
                        T tile_m3 = 0;
                        size_t index_row = row * size + i;
                        size_t index_col = col * size + j;

                        if (index_row < m3.rows() && index_col < m3.cols()) {
                            for (size_t k = 0; k < size; k++) {
                                tile_m3 += tile_m1(i, k) * tile_m2(j, k);
                            }

                            m3(index_row, index_col) += tile_m3;
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
    
    m3 = multiply_naive<T>(m1, m2);

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
