#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <cstring>

class Matrix {

    public:

        Matrix() = default;

        Matrix(size_t nrow, size_t ncol)
        : m_nrow(nrow), m_ncol(ncol)
        {
            reset_buffer(nrow, ncol);
        }

        Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec)
        : m_nrow(nrow), m_ncol(ncol)
        {
            reset_buffer(nrow, ncol);
            (*this) = vec;
        }

        Matrix & operator=(std::vector<double> const & vec)
        {
            if (size() != vec.size())
            {
                throw std::out_of_range("number of elements mismatch");
            }

            size_t k = 0;
            for (size_t i=0; i<m_nrow; ++i)
            {
                for (size_t j=0; j<m_ncol; ++j)
                {
                    (*this)(i,j) = vec[k];
                    ++k;
                }
            }

            return *this;
        }

        Matrix(Matrix const & other)
        : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
        {
            reset_buffer(other.m_nrow, other.m_ncol);
            for (size_t i=0; i<m_nrow; ++i)
            {
                for (size_t j=0; j<m_ncol; ++j)
                {
                    (*this)(i,j) = other(i,j);
                }
            }
        }

        Matrix & operator=(Matrix const & other)
        {
            if (this == &other) { return *this; }
            if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
            {
                reset_buffer(other.m_nrow, other.m_ncol);
            }
            for (size_t i=0; i<m_nrow; ++i)
            {
                for (size_t j=0; j<m_ncol; ++j)
                {
                    (*this)(i,j) = other(i,j);
                }
            }
            return *this;
        }

        Matrix(Matrix && other)
        : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
        {
            reset_buffer(0, 0);
            std::swap(m_nrow, other.m_nrow);
            std::swap(m_ncol, other.m_ncol);
            std::swap(m_buffer, other.m_buffer);
        }

        Matrix & operator=(Matrix && other)
        {
            if (this == &other) { return *this; }
            reset_buffer(0, 0);
            std::swap(m_nrow, other.m_nrow);
            std::swap(m_ncol, other.m_ncol);
            std::swap(m_buffer, other.m_buffer);
            return *this;
        }

        ~Matrix()
        {
            reset_buffer(0, 0);
        }

        double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
        double & operator() (size_t row, size_t col)       { return m_buffer[index(row, col)]; }

        static Matrix multiply_naive (const Matrix &left, const Matrix& right);
        static Matrix multiply_tiling (const Matrix &left, const Matrix& right, int cacheline);
        static Matrix multiply_mkl (const Matrix &left, const Matrix& right);

        friend std::ostream & operator<< (std::ostream & ostr, Matrix const & mat);
        friend Matrix operator*(Matrix const & mat1, Matrix const & mat2);

        size_t nrow() const { return m_nrow; }
        size_t ncol() const { return m_ncol; }

        size_t size() const { return m_nrow * m_ncol; }
        double buffer(size_t i) const { return m_buffer[i]; }
        std::vector<double> buffer_vector() const { return std::vector<double>(m_buffer, m_buffer+size()); }

        double * data() const { return m_buffer; }

    private:

        size_t index(size_t row, size_t col) const
        {
            #ifdef ROW_MAJOR
                return row * m_ncol + col;
            #else
                return row + col * m_nrow; 
            #endif
        }

        void reset_buffer(size_t nrow, size_t ncol)
        {
            if (m_buffer) { delete[] m_buffer; }
            const size_t nelement = nrow * ncol;
            if (nelement) { m_buffer = new double[nelement]; }
            else          { m_buffer = nullptr; }
            m_nrow = nrow;
            m_ncol = ncol;

            memset(m_buffer, 0, sizeof(double) * nelement);
        }

        size_t m_nrow = 0;
        size_t m_ncol = 0;
        double * m_buffer = nullptr;
};