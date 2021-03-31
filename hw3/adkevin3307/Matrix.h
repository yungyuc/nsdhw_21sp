#pragma once

#include <iostream>
#include <vector>
#include <type_traits>

template<typename T>
class Matrix {
private:
    size_t m_rows, m_cols;
    std::vector<std::vector<T>> m_matrix;

public:
    Matrix()
        : m_rows(0), m_cols(0)
    {
    }

    Matrix(size_t rows, size_t cols)
        : m_rows(rows), m_cols(cols)
    {
        this->m_matrix.resize(rows, std::vector<T>(cols, 0));
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
        for (size_t i = 0; i < this->m_matrix.size(); i++) {
            this->m_matrix[i].clear();
            this->m_matrix[i].shrink_to_fit();
        }

        this->m_matrix.clear();
        this->m_matrix.shrink_to_fit();
    }

    Matrix& operator=(Matrix const& rhs)
    {
        this->m_rows = rhs.m_rows;
        this->m_cols = rhs.m_cols;
        this->m_matrix = rhs.m_matrix;
    }

    Matrix& operator=(Matrix&& rhs)
    {
        this->m_rows = rhs.m_rows;
        this->m_cols = rhs.m_cols;
        this->m_matrix = rhs.m_matrix;
    }

    T& operator()(size_t i, size_t j)
    {
        return this->m_matrix[i][j];
    }

    const size_t rows()
    {
        return this->m_rows;
    }

    const size_t cols()
    {
        return this->m_cols;
    }

    friend std::ostream& operator<<(std::ostream& os, Matrix<T>& matrix)
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
