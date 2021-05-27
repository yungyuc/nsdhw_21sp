#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include "mkl.h"

class Matrix {
private:
    size_t m_rows, m_cols;
    std::vector<double> m_matrix;

public:
    Matrix()
        : m_rows(0), m_cols(0)
    {
    }

    Matrix(size_t rows, size_t cols)
        : m_rows(rows), m_cols(cols)
    {
        this->m_matrix.resize(this->m_rows * this->m_cols, 0);
    }

    Matrix(Matrix const& rhs)
        : m_rows(rhs.m_rows), m_cols(rhs.m_cols), m_matrix(rhs.m_matrix)
    {
    }

    Matrix(Matrix&& rhs)
        : m_rows(rhs.m_rows), m_cols(rhs.m_cols), m_matrix(std::move(rhs.m_matrix))
    {
    }

    ~Matrix()
    {
        this->m_matrix.clear();
        this->m_matrix.shrink_to_fit();
    }

    Matrix& operator=(Matrix const& rhs)
    {
        if (this != &rhs) {
            this->m_rows = rhs.m_rows;
            this->m_cols = rhs.m_cols;
            this->m_matrix = rhs.m_matrix;
        }

        return *this;
    }

    Matrix& operator=(Matrix&& rhs)
    {
        if (this != &rhs) {
            this->m_rows = rhs.m_rows;
            this->m_cols = rhs.m_cols;
            this->m_matrix = std::move(rhs.m_matrix);
        }

        return *this;
    }

    double const& operator()(size_t i, size_t j) const
    {
        return this->m_matrix[i * this->m_cols +j];
    }

    double& operator()(size_t i, size_t j)
    {
        return this->m_matrix[i * this->m_cols + j];
    }

    const double* data() const
    {
        return &(this->m_matrix[0]);
    }

    double* data()
    {
        return &(this->m_matrix[0]);
    }

    bool operator==(const Matrix& rhs) const
    {
        return (this->m_matrix == rhs.m_matrix);
    }

    constexpr size_t nrow() const
    {
        return this->m_rows;
    }

    constexpr size_t ncol() const
    {
        return this->m_cols;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
    {
        for (size_t i = 0; i < matrix.nrow(); i++) {
            for (size_t j = 0; j < matrix.ncol(); j++) {
                os << matrix(i, j) << (j == matrix.ncol() - 1 ? "" : " ");
            }

            if (i < matrix.nrow() - 1) os << '\n';
        }

        return os;
    }
};

inline Matrix multiply_naive(const Matrix& m1, const Matrix& m2)
{
    assert(m1.ncol() == m2.nrow());

    Matrix m3(m1.nrow(), m2.ncol());

    size_t rows = m1.nrow();
    size_t cols = m2.ncol();
    size_t inners = m1.ncol();

    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < cols; col++) {
            for (size_t inner = 0; inner < inners; inner++) {
                m3(row, col) += m1(row, inner) * m2(inner, col);
            }
        }
    }

    return m3;
}

inline Matrix multiply_tile(const Matrix& m1, const Matrix& m2, size_t size)
{
    assert(m1.ncol() == m2.nrow());

    Matrix m3(m1.nrow(), m2.ncol());

    for (size_t row = 0; row < m1.nrow(); row += size) {
        for (size_t col = 0; col < m2.ncol(); col += size) {
            for (size_t inner = 0; inner < m1.ncol(); inner += size) {
                // tile
                for (size_t k = inner; k < std::min(m1.ncol(), inner + size); k++) {
                    for (size_t i = row; i < std::min(m1.nrow(), row + size); i++) {
                        for (size_t j = col; j < std::min(m2.ncol(), col + size); j++) {
                            m3(i, j) += m1(i, k) * m2(k, j);
                        }
                    }
                }
            }
        }
    }

    return m3;
}

inline Matrix multiply_mkl(Matrix& m1, Matrix& m2)
{
    assert(m1.ncol() == m2.nrow());

    Matrix m3(m1.nrow(), m2.ncol());

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m1.nrow(), m2.ncol(), m1.ncol(), 1, m1.data(), m1.ncol(), m2.data(), m2.ncol(), 0, m3.data(), m3.ncol());

    return m3;
}
