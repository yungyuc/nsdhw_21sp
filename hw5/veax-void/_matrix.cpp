// #include <pybind11/pybind11.h>
// #include <pybind11/operators.h>
// #include <pybind11/numpy.h>
// #include <mkl.h>

// #include <new>
// #include <limits>
// #include <memory>
// #include <string>
// #include <vector>
// #include <iostream>

// #include "_matrix.hpp"

#include <iostream>
#include <algorithm>
#include "mkl.h"

#include "_matrix.hpp"

// default contructor
Matrix::Matrix(size_t nrow, size_t ncol)
    : m_nrow(nrow), m_ncol(ncol), m_buffer(nrow * ncol, 0)
{
}

// copy constructor
Matrix::Matrix(Matrix const & other)
    : m_nrow(other.m_nrow), m_ncol(other.m_ncol), m_buffer(other.m_nrow * other.m_ncol, 0)
{
    for(size_t i=0; i < m_nrow; ++i)
    {
        const size_t base_t = i*m_ncol;
        const size_t base_s = i*other.m_ncol;
        for (size_t j=0; j < m_ncol; ++j)
            if (i >= other.m_nrow || j >= other.m_ncol) m_buffer.at(base_t + j) = 0;
            else m_buffer.at(base_t + j) = other.m_buffer.at(base_s + j);
    }
}

// move constructor
Matrix::Matrix(Matrix && other)
    : m_nrow(other.m_nrow), m_ncol(other.m_ncol), m_buffer(other.m_nrow * other.m_ncol, 0)
{
    other.m_buffer.swap(m_buffer);
}

Matrix::Matrix(std::vector<std::vector<double>> const & other)
    : m_nrow(other.size()), m_ncol(other[0].size()), m_buffer(other.size() * other[0].size(), 0)
{
    for(const auto &v: other)
        m_buffer.insert(m_buffer.end(), v.begin(), v.end()); 
}

inline void validate_multiplication(Matrix const &mat1, Matrix const &mat2)
{
    if (mat1.m_ncol != mat2.m_nrow)
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }
}

Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2)
{
    validate_multiplication(mat1, mat2);

    // New matrix to be returned
    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t i=0; i<ret.nrow(); ++i)
    {
        for (size_t k=0; k<ret.ncol(); ++k)
        {
            for (size_t j=0; j<mat1.ncol(); ++j)
            {
                ret(i, k) += mat1(i,j) * mat2(j,k);
            }
        }
    }
    return ret;
};

Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2)
{
    validate_multiplication(mat1, mat2);

    // New matrix to be returned
    Matrix ret(mat1.m_nrow, mat2.m_ncol);

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
        mat1.m_nrow, mat2.m_ncol, mat1.m_ncol, 1.0,
        mat1.m_buffer.data(), mat1.m_ncol,
        mat2.m_buffer.data(), mat2.m_ncol,
        0.0,
        ret.m_buffer.data(), mat2.m_ncol
    );
    return ret;
};

Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, const int tsize)
{
    validate_multiplication(mat1, mat2);

    const size_t f_row = mat1.m_nrow;
    const size_t f_col = mat1.m_ncol;
    const size_t s_col = mat2.m_ncol;

    Matrix ret(f_row, s_col);

    const size_t tile = tsize;

    for (size_t k_tile = 0; k_tile < f_col; k_tile += tile) {
        const size_t k_tile_cur = std::min( f_col, (k_tile + tile) );

        for (size_t i_tile = 0; i_tile < f_row; i_tile += tile) {
            const size_t i_tile_cur = std::min( f_row, (i_tile + tile) );

            for (size_t j_tile = 0; j_tile < s_col; j_tile += tile) {
                const size_t j_tile_cur = std::min( s_col, (j_tile + tile) );

                for (size_t k = k_tile; k < k_tile_cur; ++k) {
                    for (size_t i = i_tile; i < i_tile_cur; ++i) {
                        const double left = mat1(i, k);
                        for (size_t j = j_tile; j < j_tile_cur; ++j) {
                            ret(i, j) += left * mat2(k, j);
                        }
                    }
                }
            }
        }
    }    
    return ret;
}