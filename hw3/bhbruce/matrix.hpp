// #pragma once
#include <cstddef>
#include <vector>
class Matrix
{
public:
    Matrix() // default constructor
        :m_nrow(0), m_ncol(0)
    {}
    Matrix(size_t nrow, size_t ncol)
        :m_nrow(nrow), m_ncol(ncol)
    {
        m_vector.clear();
        m_vector.resize(this->size(), 0);
    }
    Matrix(Matrix const & other) // copy con.
        :m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        m_vector.resize(m_nrow, m_ncol);
        for (size_t i=0; i<m_nrow; i++)
            for (size_t j=0; j<m_ncol; j++)
                (*this)(i,j) = other(i,j);
    }
    Matrix & operator=(Matrix const & other) // copy assignment operator
    {
        if(this == &other)
            return *this;
        m_vector.resize(m_nrow, m_ncol);
        for (size_t i=0; i<m_nrow; i++)
            for (size_t j=0; j<m_ncol; j++)
                (*this)(i,j) = other(i,j);
        return *this;
    }
    Matrix(Matrix && other) // move con.
        :m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        this->m_vector = std::move(other.m_vector);
    }
    Matrix & operator=(Matrix && other) // move assignment operator
    {
        if(this == &other)
            return *this;
        this->m_ncol = other.m_ncol;
        this->m_nrow = other.m_nrow;
        this->m_vector = std::move(other.m_vector);
        return *this;

    }
    ~Matrix() // destructor
    {
        m_vector.clear();
        m_vector.shrink_to_fit();
    }
    size_t size() const // getter (return value)
    {
        return m_nrow * m_ncol;
    }
    const double* addr() const {return &m_vector[0];} // getter (ret const ref)
    size_t const & nrow() const {return m_nrow;} // getter (ret const ref)
    size_t const & ncol() const {return m_ncol;} // getter

    friend bool operator == (const Matrix& lhs, const Matrix& rhs)
    {
        return (lhs.m_ncol==rhs.m_ncol)&&(lhs.m_nrow==rhs.m_nrow)&&(lhs.m_vector==rhs.m_vector);
    }

    double const & operator() (size_t row, size_t col) const // getter
    {
        return m_vector[_index(row, col)];
    }
    double & operator() (size_t row, size_t col)
    {
        return m_vector[_index(row, col)];
    }
    //float & nrow(size_t it); // setter
    //float & y(size_t it); // setter
private:
    size_t _index(size_t row, size_t col) const
    {
        return m_ncol * row + col;
    }
    std::vector<double> m_vector;
    size_t m_nrow, m_ncol;
    // Member data.
}; /* end class Matrix */
