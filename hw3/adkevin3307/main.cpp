#include <iostream>
#include <cassert>
#include <random>
#include <functional>
#include <ctime>
#include <utility>
#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "Matrix.h"

using namespace std;

template<typename T>
Matrix<T> multiply_naive(Matrix<T>& m1, Matrix<T>& m2)
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
Matrix<T> multiply_tile(Matrix<T>& m1, Matrix<T>& m2, size_t size)
{
    assert(m1.cols() == m2.rows());

    Matrix<T> m3(m1.rows(), m2.cols());

    size_t tile_rows = m1.rows() / size;
    size_t tile_cols = m2.cols() / size;
    size_t tile_inners = m1.cols() / size;

    Matrix<T> tile_m1(size, size);
    Matrix<T> tile_m2(size, size);
    Matrix<T> tile_m3(size, size);

    for (size_t row = 0; row < tile_rows; row++) {
        for (size_t col = 0; col < tile_cols; col++) {
            for (size_t inner = 0; inner < tile_inners; inner++) {
                for (size_t i = 0; i < size; i++) {
                    for (size_t j = 0; j < size; j++) {
                        tile_m1(i, j) = m1(row * size + i, inner * size + j);
                        tile_m2(i, j) = m2(inner * size + j, col * size + i);
                    }
                }

                for (size_t i = 0; i < size; i++) {
                    for (size_t j = 0; j < size; j++) {
                        for (size_t k = 0; k < size; k++) {
                            m3(row * size + i, col * size + j) += tile_m1(i, k) * tile_m2(j, k);
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
}

PYBIND11_MODULE(_matrix, m)
{
    pybind11::class_<Matrix<int>>(m, "Matrix")
        .def(pybind11::init<size_t, size_t>())
        .def("__getitem__", [](Matrix<int>& m, pair<size_t, size_t> index) {
            return m(index.first, index.second);
        })
        .def("__setitem__", [](Matrix<int>& m, pair<size_t, size_t> index, int value) {
            m(index.first, index.second) = value;
        })
        .def("__str__", [](Matrix<int>& m) {
            stringstream ss;
            ss << m;

            return ss.str();
        })
        .def("rows", &Matrix<int>::rows)
        .def("cols", &Matrix<int>::cols);

    m.def("multiply_naive", multiply_naive<int>, "Matrix multiply with naive method.");
    m.def("multiply_tile", multiply_tile<int>, "Matrix multiply with tile method.");
    // m.def("multiply_mkl", &multiply_mkl, "Matrix multiply with mkl method.");
}
