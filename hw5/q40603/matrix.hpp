#include <iostream>
#include <vector>
#include <stdexcept>
#include <mkl.h>
#include <math.h>


bool cmpf(float A, float B, float epsilon = 0.005f)
{
    return (fabs(A - B) < epsilon);
}

class Matrix {
public:
    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol) {
        reset_buffer(nrow, ncol);
    }

    Matrix(Matrix const &other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol) {
        reset_buffer(other.m_nrow, other.m_ncol);
        for (size_t i = 0; i < m_nrow; ++i) {
            for (size_t j = 0; j < m_ncol; ++j) {
                (*this)(i,j) = other(i,j);
            }
        }
    }

    Matrix(std::vector<std::vector<double>> const &v) {
        m_nrow = v.size();
        m_ncol = v[0].size();
        reset_buffer(m_nrow, m_ncol);
        for (size_t i = 0; i < m_nrow; ++i) {
            for (size_t j = 0; j < m_ncol; ++j) {
                (*this)(i,j) = v[i][j];
            }
        }
    }

    ~Matrix() {
        reset_buffer(0, 0);
    }

    double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[index(row, col)]; }

    Matrix &operator=(Matrix const &other) {
        if (this == &other) {
            return *this;
        }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) {
            reset_buffer(other.m_nrow, other.m_ncol);
        }
        for (size_t i = 0; i < m_nrow; ++i) {
            for (size_t j = 0; j < m_ncol; ++j) {
                (*this)(i,j) = other(i,j);
            }
        }
        return *this;
    }

    bool operator==(Matrix const &other) const{
        if (this == &other) {
            return true;
        }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) {
            return false;
        }
        for (size_t i = 0; i < m_nrow; ++i) {
            for (size_t j = 0; j < m_ncol; ++j) {
                if (! cmpf((*this)(i,j), other(i,j))) {
                    return false;
                }
            }
        }
        return true;
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    size_t size() const { return m_nrow * m_ncol; }

    double buffer(size_t i) const { return m_buffer[i]; }
    double *data() const { return m_buffer; }

    std::vector<double> buffer_vector() const { return std::vector<double>(m_buffer, m_buffer + size()); }

private:
    size_t index(size_t row, size_t col) const {
        return row + col * m_nrow;
    }
    void reset_buffer(size_t nrow, size_t ncol) {
        if (m_buffer) {
            delete[] m_buffer;
        }
        const size_t nelement = nrow * ncol;
        if (nelement) {
            m_buffer = new double[nelement];
            for (size_t i = 0; i < nelement; ++i) {
                m_buffer[i] = 0.0;
            }
        } else {
            m_buffer = nullptr;
        }
        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double *m_buffer = nullptr;
};

Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2) {
    if (mat1.ncol() != mat2.nrow()) {
        throw std::out_of_range("Incorrect dimensions for matrix multiplication");
    }

    size_t nrow = mat1.nrow();
    size_t ncol = mat2.ncol();

    Matrix ret(nrow, ncol);

    for (size_t i = 0; i < nrow; ++i) {
        for (size_t k = 0; k < ncol; ++k) {
            for (size_t j = 0; j < mat1.ncol(); ++j) {
                ret(i,k) += mat1(i,j) * mat2(j,k);
            }
        }
    }

    return ret;
}

inline size_t edge(size_t n, size_t ori) {
    if (n > ori) {
        return ori;
    }
    return n;
}

Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t tile_size) {
    if (mat1.ncol() != mat2.nrow()) {
        throw std::out_of_range("Incorrect dimensions for matrix multiplication");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t i = 0; i < mat1.nrow(); i += tile_size) {
        for (size_t k = 0; k < mat2.ncol(); k += tile_size) {
            for (size_t j = 0; j <  mat1.ncol(); j += tile_size) {
                // multiply_naive()
                for (size_t tile_j = j; tile_j < edge(j + tile_size, mat1.ncol()); ++tile_j) {
                    for (size_t tile_i = i; tile_i < edge(i + tile_size, mat1.nrow()); ++tile_i) {
                        for (size_t tile_k = k; tile_k < edge(k + tile_size,mat2.ncol()); ++tile_k) {
                            ret(tile_i, tile_k) += mat1(tile_i, tile_j) * mat2(tile_j, tile_k);
                        }
                    }
                }
            }
        }
    }

    return ret;
}


Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2) {
    Matrix ret = Matrix(mat1.nrow(), mat2.ncol());

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, mat1.nrow(), mat2.ncol(), mat1.ncol(), 1, mat1.data(), mat1.ncol(), mat2.data(), mat2.ncol(), 0, ret.data(), mat2.ncol());

    return ret;
}