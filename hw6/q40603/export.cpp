#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>

#include "matrix.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_matrix, m) {
    m.doc() = "matrix mutiplication module";
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);

    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())        
        .def(py::init<size_t, size_t>())
        .def("buffer", &Matrix::buffer)
        .def("size", &Matrix::size)
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def_property("array", [](Matrix &m) {
            return py::array_t<double>(
                { m.nrow(), m.ncol() },
                { sizeof(double) * m.ncol(), sizeof(double) },
                m.data(),
                py::cast(m)
            );
        }, nullptr)
        .def("__eq__", &Matrix::operator==)
        .def("__getitem__", [](const Matrix &mat, std::pair<size_t, size_t> i){
            return mat(i.first, i.second);
        })
        .def("__setitem__", [](Matrix &mat, std::pair<size_t, size_t> i, double val){
            mat(i.first, i.second) = val;
        });
}
