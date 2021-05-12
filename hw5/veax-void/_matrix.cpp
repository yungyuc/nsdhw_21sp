#include <mkl.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include "_matrix.hpp"

#include <mkl.h>

std::ostream & operator<< (std::ostream & ostr, Matrix const & mat)
{
    for (size_t i=0; i<mat.nrow(); ++i)
    {
        ostr << std::endl << " ";
        for (size_t j=0; j<mat.ncol(); ++j)
        {
            ostr << " " << std::setw(2) << mat(i, j);
        }
    }

    return ostr;
}

Matrix operator*(Matrix const & mat1, Matrix const & mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t i=0; i<ret.nrow(); ++i)
    {
        for (size_t k=0; k<ret.ncol(); ++k)
        {
            double v = 0;
            for (size_t j=0; j<mat1.ncol(); ++j)
            {
                v += mat1(i,j) * mat2(j,k);
            }
            ret(i,k) = v;
        }
    }

    return ret;
}


// Self Implementation starts here

Matrix Matrix::multiply_naive (const Matrix &left, const Matrix& right) 
{
    if (left.ncol() != right.nrow()) {
        throw std::out_of_range("Error Size");
    }

    Matrix ret(left.nrow(), right.ncol());

    for(int i = 0; i < ret.nrow(); ++i) {
        for (int j = 0; j < ret.ncol(); ++j) {
            double v = 0;
            for(int k = 0; k < left.ncol(); ++k) {
                v += left(i, k) * right(k, j);
            }
            ret(i, j) = v;
        }
    }

    return ret;
}

Matrix Matrix::multiply_tiling (const Matrix &left, const Matrix& right, int cacheline) 
{
    if (left.ncol() != right.nrow()) {
        throw std::out_of_range("Error Size");
    }

    Matrix ret(left.nrow(), right.ncol());
    for(size_t i = 0; i < left.nrow(); ++i) {
        for (size_t j = 0; j < right.ncol(); ++j) {
            ret(i, j) = 0.0; // explicit clean the matrix since we "add" value to ret not "assign"
        }
    }

    const int blocksize = cacheline / sizeof(double);

    for(int i = 0; i < ret.nrow(); i += blocksize) {
        const bool isExceedBound = ((i + blocksize) > ret.nrow());
        const int index_i = isExceedBound ? ret.nrow() : (i + blocksize); // clamp the size if out of bound
        
        for (int j = 0; j < ret.ncol(); j += blocksize) {
            const bool isExceedBound = ((j + blocksize) > ret.ncol());
            const int index_j = isExceedBound ? ret.ncol() : (j + blocksize);

            for (int k = 0; k < left.ncol(); k += blocksize) {
                const bool isExceedBound = ((k + blocksize) > left.ncol());
                const int index_k = isExceedBound ? left.ncol() : (k + blocksize);

                // do multiplication in blocks
                for (int tile_i = i; tile_i < index_i; ++tile_i) {
                    for (int tile_j = j; tile_j < index_j; ++tile_j) {
                        double v = 0;
                        for (int tile_k = k; tile_k < index_k; ++tile_k) {
                            v += left(tile_i, tile_k) * right(tile_k, tile_j);
                        }
                        ret(tile_i, tile_j) += v; // use += instead of =
                    }
                }

            }
        }
    }

    return ret;
}

Matrix Matrix::multiply_mkl (const Matrix &left, const Matrix& right) 
{
    if (left.ncol() != right.nrow()) {
        throw std::out_of_range("Error Size");
    }
    
    Matrix ret(left.nrow(), right.ncol());

    // Ref: https://software.intel.com/en-us/mkl-tutorial-c-multiplying-matrices-using-dgemm

    int m = left.nrow();
    int k = left.ncol();
    int n = right.ncol();

    #ifdef ROW_MAJOR
        cblas_dgemm(        
            CblasRowMajor,        
            CblasNoTrans,   //
            CblasNoTrans,   //     
            m,              // M
            n,              // N
            k,              // K
            1.0,            // ALPHA,
            left.data(),    // A
            left.ncol(),    // LDA,
            right.data(),   // B
            right.ncol(),   // LDB
            0.0,            // BETA,
            ret.data(),     // C
            ret.ncol()      // LDC
        );
    #else
        cblas_dgemm(        
            CblasColMajor,        
            CblasNoTrans,   //
            CblasNoTrans,   //     
            m,              // M
            n,              // N
            k,              // K
            1.0,            // ALPHA,
            left.data(),    // A
            left.nrow(),    // LDA,
            right.data(),   // B
            right.nrow(),   // LDB        
            0.0,            // BETA,
            ret.data(),     // C
            ret.nrow()      // LDC
        );
    #endif
    return ret;
}