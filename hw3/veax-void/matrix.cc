
#include "matrix.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <algorithm>

#include "mkl.h"

namespace py = pybind11;

Matrix::Matrix(const std::vector<std::vector<double>> &m) {
  m_nrow = m.size();
  m_ncol = m.back().size();
  for (auto v : m) {
    std::copy(v.begin(), v.end(), std::back_inserter(m_buffer));
  }
}

bool Matrix::operator==(const Matrix &other) {
  if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) {
    return false;
  }
  for (size_t i = 0; i < m_nrow; ++i) {
    for (size_t j = 0; j < m_ncol; ++j) {
      if ((*this)(i, j) != other(i, j)) {
        return false;
      }
    }
  }
  return true;
}

Matrix multiply_naive(const Matrix &m1, const Matrix &m2) {
  if (m1.ncol() != m2.nrow()) {
    throw std::out_of_range(
        "the number of first matrix column "
        "differs from that of second matrix row");
  }

  Matrix m3(m1.nrow(), m2.ncol());
  for (size_t i = 0; i < m1.nrow(); ++i) {
    for (size_t j = 0; j < m2.ncol(); ++j) {
      double v = 0.0;
      for (size_t k = 0; k < m1.ncol(); ++k) {
        v += m1(i, k) * m2(k, j);
      }
      m3(i, j) = v;
    }
  }
  return m3;
}

Matrix multiply_tile(const Matrix &m1, const Matrix &m2, size_t tsize) {
  if (m1.ncol() != m2.nrow()) {
    throw std::out_of_range(
        "the number of first matrix column "
        "differs from that of second matrix row");
  }

  Matrix m3(m1.nrow(), m2.ncol());
  for (size_t i = 0; i < m1.nrow(); i += tsize) {
    for (size_t j = 0; j < m2.ncol(); j += tsize) {
      size_t edgei = std::min(m1.nrow(), i + tsize);
      size_t edgej = std::min(m2.ncol(), j + tsize);
      for (size_t k = 0; k < m1.ncol(); k += tsize) {
        size_t edgek = std::min(m1.ncol(), k + tsize);
        for (size_t kk = k; kk < edgek; ++kk) {
          for (size_t ii = i; ii < edgei; ++ii) {
            for (size_t jj = j; jj < edgej; ++jj) {
              m3(ii, jj) += m1(ii, kk) * m2(kk, jj);
            }
          }
        }
      }
    }
  }
  return m3;
}

Matrix multiply_mkl(const Matrix &m1, const Matrix &m2) {
  if (m1.ncol() != m2.nrow()) {
    throw std::out_of_range(
        "the number of first matrix column "
        "differs from that of second matrix row");
  }

  Matrix m3(m1.nrow(), m2.ncol());
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m1.nrow(), m2.ncol(),
              m1.ncol(), 1.0, m1.buffer(), m1.ncol(), m2.buffer(), m2.ncol(),
              0.0, m3.buffer(), m3.ncol());
  return m3;
}

PYBIND11_MODULE(_matrix, m) {
  m.def("multiply_naive", &multiply_naive);
  m.def("multiply_tile", &multiply_tile);
  m.def("multiply_mkl", &multiply_mkl);

  py::class_<Matrix>(m, "Matrix")
      .def(py::init<size_t, size_t>())
      .def(py::init<const std::vector<std::vector<double>> &>())
      .def_property_readonly("nrow", &Matrix::nrow)
      .def_property_readonly("ncol", &Matrix::ncol)
      .def("__eq__", &Matrix::operator==)
      .def("__getitem__",
           [](const Matrix &m, std::array<int, 2> i) { return m(i[0], i[1]); })
      .def("__setitem__", [](Matrix &m, std::array<int, 2> i, double v) {
        m(i[0], i[1]) = v;
      });
}
