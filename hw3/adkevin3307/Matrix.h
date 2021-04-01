#pragma once

#include <iostream>
#include <vector>

template<typename T>
class Matrix {
private:
    size_t m_rows, m_cols;
    std::vector<T> m_matrix;

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
    {
        this->m_rows = rhs.m_rows;
        this->m_cols = rhs.m_cols;
        this->m_matrix = rhs.m_matrix;
    }

    Matrix(Matrix&& rhs)
    {
        this->m_rows = rhs.m_rows;
        this->m_cols = rhs.m_cols;
        this->m_matrix = rhs.m_matrix;
    }

    ~Matrix()
    {
        this->m_matrix.clear();
        this->m_matrix.shrink_to_fit();
    }

    Matrix& operator=(Matrix const& rhs)
    {
        this->m_rows = rhs.m_rows;
        this->m_cols = rhs.m_cols;
        this->m_matrix = rhs.m_matrix;

        return *this;
    }

    Matrix& operator=(Matrix&& rhs)
    {
        this->m_rows = rhs.m_rows;
        this->m_cols = rhs.m_cols;
        this->m_matrix = rhs.m_matrix;

        return *this;
    }

    T const& operator()(size_t i, size_t j) const
    {
        return this->m_matrix[i * this->m_cols +j];
    }

    T& operator()(size_t i, size_t j)
    {
        return this->m_matrix[i * this->m_cols + j];
    }

    const T* data() const
    {
        return &(this->m_matrix[0]);
    }

    T* data()
    {
        return &(this->m_matrix[0]);
    }

    bool operator==(const Matrix& rhs) const
    {
        return this->m_matrix == rhs.m_matrix;
    }

    const size_t rows() const
    {
        return this->m_rows;
    }

    const size_t cols() const
    {
        return this->m_cols;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix)
    {
        for (size_t i = 0; i < matrix.rows(); i++) {
            for (size_t j = 0; j < matrix.cols(); j++) {
                os << matrix(i, j) << (j == matrix.cols() - 1 ? "" : " ");
            }

            if (i < matrix.rows() - 1) os << '\n';
        }

        return os;
    }
};
