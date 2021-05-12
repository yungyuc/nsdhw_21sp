#include <mkl.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <functional>
#include <atomic>
#include "matrix.hpp"

Matrix::Matrix(size_t row, size_t col) : nrow(row), ncol(col){
    if (col * row == 0)
    {
        throw std::out_of_range("number of elements mismatch");
    }
    // init to 0
    data.resize( nrow * ncol , 0 );
}

Matrix& Matrix::operator=(Matrix &other){
    nrow = other.nrow;
    ncol = other.ncol;
    data.assign( other.data.begin(), other.data.end() );
    return *this;
}

Matrix::Matrix(Matrix && other) : nrow(other.nrow), ncol(other.ncol) {
    data.clear();
    std::swap(data, other.data);
}

Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2)
{

    if (mat1.ncol != mat2.nrow)
        throw std::invalid_argument("Input matrix rank didn't match");

    Matrix ret(mat1.nrow, mat2.ncol);

    cblas_dgemm(
        CblasRowMajor /* const CBLAS_LAYOUT Layout */
        ,
        CblasNoTrans /* const CBLAS_TRANSPOSE transa */
        ,
        CblasNoTrans /* const CBLAS_TRANSPOSE transb */
        ,
        mat1.m_row() /* const MKL_INT m */
        ,
        mat2.m_col() /* const MKL_INT n */
        ,
        mat1.m_col() /* const MKL_INT k */
        ,
        1.0 /* const double alpha */
        ,
        mat1.data.data() /* const double *a */
        ,
        mat1.m_col() /* const MKL_INT lda */
        ,
        mat2.data.data() /* const double *b */
        ,
        mat2.m_col() /* const MKL_INT ldb */
        ,
        0.0 /* const double beta */
        ,
        ret.data.data() /* double * c */
        ,
        ret.m_col() /* const MKL_INT ldc */
    );

    return ret;
}

bool operator==(Matrix const &mat1, Matrix const &mat2)
{
    if ((mat1.ncol != mat2.ncol) && (mat1.nrow != mat2.ncol))
    {
        return false;
    }

    for (size_t i = 0; i < mat1.nrow; ++i)
    {
        for (size_t j = 0; j < mat1.ncol; ++j)
        {
            if (mat1(i, j) != mat2(i, j))
            {
                return false;
            }
        }
    }

    return true;
}

/*
 * Direct naive matrix matrix multiplication.
 */

Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2)
{
    if (mat1.ncol != mat2.nrow)
    {
        throw std::out_of_range("number of elements mismatch");
    }

    Matrix ret(mat1.nrow, mat2.ncol);

    for (size_t ret_row = 0; ret_row < mat1.nrow; ++ret_row)
    {
        for (size_t ret_col = 0; ret_col < mat2.ncol; ++ret_col)
        {
            for (size_t index = 0; index < mat1.ncol; index++)
            {
                ret(ret_row, ret_col) += mat1(ret_row, index) * mat2(index, ret_col);
            }
        }
    }

    return ret;
}

Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t tsize)
{
    if (mat1.ncol != mat2.nrow)
    {
        throw std::out_of_range("number of elements mismatch");
    }

    Matrix ret(mat1.nrow, mat2.ncol);
    Matrix Tmat2(mat2.ncol, mat2.nrow);

    // perform mat2 transpose
    for (size_t Tmat2_row = 0; Tmat2_row < Tmat2.nrow; ++Tmat2_row)
    {
        for (size_t Tmat2_col = 0; Tmat2_col < Tmat2.ncol; ++Tmat2_col)
        {
            Tmat2(Tmat2_row, Tmat2_col) = mat2(Tmat2_col, Tmat2_row);
        }
    }

    // choose tile to start
    for (size_t tile_row = 0; tile_row * tsize < mat1.nrow; ++tile_row)
    {
        for (size_t tile_col = 0; tile_col * tsize < mat2.ncol; ++tile_col)
        {
            for (size_t tile_mul_index = 0; tile_mul_index * tsize < mat1.ncol; tile_mul_index++)
            {
                // start calculate the return tile
                // tile(tile_row, tile_col) = mat1(tile_row, tile_mul_index) * Tmat2(tile_col, tile_mul_index)
                for (size_t in_tile_mul_index = tile_mul_index * tsize; in_tile_mul_index < std::min((tile_mul_index + 1) * tsize, mat1.ncol); ++in_tile_mul_index)
                {
                    for (size_t in_tile_row = tile_row * tsize; in_tile_row < std::min((tile_row + 1) * tsize, mat1.nrow); ++in_tile_row)
                    {
                        for (size_t in_tile_col = tile_col * tsize; in_tile_col < std::min((tile_col + 1) * tsize, mat2.ncol); ++in_tile_col)
                        {
                            ret(in_tile_row, in_tile_col) += mat1(in_tile_row, in_tile_mul_index) * Tmat2(in_tile_col, in_tile_mul_index);
                        }
                    }
                }
            }
        }
    }

    return ret;
}