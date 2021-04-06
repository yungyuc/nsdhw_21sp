#include "matrix.hpp"
#include <stdexcept>
#include <mkl.h>
#include <iostream>
#include <sstream>
#include <utility>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

void check_validate_mul(Matrix const & matrix1, Matrix const & matrix2)
{
    if (matrix1.ncol() != matrix2.nrow())
        throw std::out_of_range("Num. of columns in M1 must equal"
                                "to num. of rows in M2.");
}

Matrix multiply_naive(Matrix const & matrix1, Matrix const & matrix2)
{
    check_validate_mul(matrix1, matrix2);
    Matrix ret(matrix1.nrow(), matrix2.ncol());
    const size_t nrow1 = matrix1.nrow();
    const size_t ncol1 = matrix1.ncol();
    const size_t ncol2 = matrix2.ncol();
    for (size_t i=0; i<nrow1; i++)
    {
        for (size_t j=0; j<ncol2; j++)
        {
            double vec=0;
            for (size_t k=0; k<ncol1; k++)
            {

                vec += matrix1(i,k) * matrix2(k,j);
            }
            ret(i,j) = vec;
        }
    }
    return ret;
}


// Use MKL for the matrix multiplication.
Matrix multiply_mkl(Matrix const & matrix1, Matrix const & matrix2)
{
    mkl_set_num_threads(1);

    check_validate_mul(matrix1, matrix2);
    Matrix ret(matrix1.nrow(), matrix2.ncol());
    //ret = multiply_naive(matrix1, matrix2);
    cblas_dgemm(
        CblasRowMajor /* const CBLAS_LAYOUT Layout */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transa */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transb */
      , matrix1.nrow() /* const MKL_INT m */
      , matrix2.ncol() /* const MKL_INT n */
      , matrix1.ncol() /* const MKL_INT k */
      , 1.0 /* const double alpha */
      , matrix1.addr() /* const double *a */
      , matrix1.ncol() /* const MKL_INT lda */
      , matrix2.addr() /* const double *b */
      , matrix2.ncol() /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , ret.addr() /* double * c */
      , ret.ncol() /* const MKL_INT ldc */
    )
    return ret;
}

Matrix multiply_tile(Matrix const & matrix1, Matrix const & matrix2, size_t tile_size)
{
    check_validate_mul(matrix1, matrix2);
    Matrix ret(matrix1.nrow(), matrix2.ncol());
    if (tile_size == 0)
        return multiply_naive(matrix1, matrix2);
    const size_t nrow1 = matrix1.nrow();
    const size_t ncol1 = matrix1.ncol();
    const size_t ncol2 = matrix2.ncol();

    const size_t ntrow1 = nrow1 / tile_size;
    const size_t ntcol1 = ncol1 / tile_size;
    const size_t ntcol2 = ncol2 / tile_size;

    for (size_t it=0; it<=ntrow1; it++)
    {
        for (size_t kt=0; kt<=ntcol2; kt++)
        {
            for (size_t jt=0; jt<=ntcol1; jt++)
            {
                // For each tile perform below
                for (size_t x=it*tile_size; x < std::min((it+1)*tile_size, nrow1); x++)
                {
                    for (size_t y=kt*tile_size; y < std::min((kt+1)*tile_size, ncol2); y++)
                    {
                        double vec=0;
                        for (size_t z=jt*tile_size; z < std::min((jt+1)*tile_size, ncol1); z++)
                        {

                            vec += matrix1(x, z) * matrix2(z, y);
                                                                                                                          }
                        ret(x,y) += vec;
                    }
                }
            }
        }
    }
    return ret;
}

PYBIND11_MODULE(_matrix, m)
{
    m.def("multiply_naive", multiply_naive);
    m.def("multiply_tile", multiply_tile);
    m.def("multiply_mkl", multiply_mkl);

    pybind11::class_<Matrix>(m, "Matrix")
        .def(pybind11::init<size_t, size_t>())
        .def("__getitem__", [](Matrix& m, std::pair<size_t, size_t> index)
        {
            return m(index.first, index.second);
        })
        .def("__setitem__", [](Matrix& m, std::pair<size_t, size_t> index, double value)
        {
            m(index.first, index.second) = value;
        })
        .def("__str__", [](Matrix& m)
        {
            std::stringstream ss;
	        for (size_t i=0; i<m.nrow();i++)
	        {
		        for (size_t j=0; j< m.ncol();j++)
                {
                    ss<< m(i,j)<<" ";
                }
                ss << std::endl;
	        }
            return ss.str();
        })
        .def(pybind11::self == pybind11::self)
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol);

}

