#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <mkl.h>
#include <assert.h>
#include <vector>
#include <memory>
#include <new>
#include <limits>
#include <string>

namespace py = pybind11;

struct ByteCounterImpl
{

    std::size_t allocated = 0;
    std::size_t deallocated = 0;
    std::size_t refcount = 0;

}; /* end struct ByteCounterImpl */

class ByteCounter
{

public:

    ByteCounter()
      : m_impl(new ByteCounterImpl)
    { incref(); }
/*
    ByteCounter(ByteCounter const & other)
      : m_impl(other.m_impl)
    { incref(); }

    ByteCounter & operator=(ByteCounter const & other)
    {
        if (&other != this)
        {
            decref();
            m_impl = other.m_impl;
            incref();
        }

        return *this;
    }

    ByteCounter(ByteCounter && other)
      : m_impl(other.m_impl)
    { incref(); }

    ByteCounter & operator=(ByteCounter && other)
    {
        if (&other != this)
        {
            decref();
            m_impl = other.m_impl;
            incref();
        }

        return *this;
    }
*/
    ~ByteCounter() { decref(); }

    void swap(ByteCounter & other)
    {
        std::swap(m_impl, other.m_impl);
    }

    void increase(std::size_t amount)
    {
        m_impl->allocated += amount;
    }

    void decrease(std::size_t amount)
    {
        m_impl->deallocated += amount;
    }

    std::size_t bytes() const { return m_impl->allocated - m_impl->deallocated; }
    std::size_t allocated() const { return m_impl->allocated; }
    std::size_t deallocated() const { return m_impl->deallocated; }
    /* This is for debugging. */
    std::size_t refcount() const { return m_impl->refcount; }

private:

    void incref() { ++m_impl->refcount; }

    void decref()
    {
        if (nullptr == m_impl)
        {
            // Do nothing.
        }
        else if (1 == m_impl->refcount)
        {
            delete m_impl;
            m_impl = nullptr;
        }
        else
        {
            --m_impl->refcount;
        }
    }

    ByteCounterImpl * m_impl;

}; /* end class ByteCounter */

template <class T>
struct MyAllocator
{

    using value_type = T;

    // Just use the default constructor of ByteCounter for the data member
    // "counter".
    MyAllocator() = default;

//    template <class U> constexpr
/*
    MyAllocator(const MyAllocator<U> & other) noexcept
    {
        counter = other.counter;
    }
*/

    T * allocate(std::size_t n)
    {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
        {
            throw std::bad_alloc();
        }
        const std::size_t bytes = n*sizeof(T);
        T * p = static_cast<T *>(std::malloc(bytes));
        if (p)
        {
            counter.increase(bytes);
            return p;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    void deallocate(T* p, std::size_t n) noexcept
    {
        std::free(p);

        const std::size_t bytes = n*sizeof(T);
        counter.decrease(bytes);
    }

    ByteCounter counter;

}; /* end struct MyAllocator */

MyAllocator<double> alloc;

std::size_t bytes() { return alloc.counter.bytes(); }
std::size_t allocated() { return alloc.counter.allocated(); }
std::size_t deallocated() { return alloc.counter.deallocated(); }

class Matrix {
   public:
        size_t nrow;
        size_t ncol;
        double* m_buffer;
/*
	MyAllocator<size_t> alloc;
	std::vector<size_t, MyAllocator<size_t>> vec1(alloc);
*/
        Matrix(size_t _nrow, size_t _ncol)
                : nrow(_nrow), ncol(_ncol) {
                size_t nelement = nrow * ncol;
                m_buffer = new double[nelement];
                memset(m_buffer, 0, nelement * sizeof(double));
        }


        ~Matrix() {}

        double* data() const { return m_buffer; }
        double operator()(size_t row, size_t col) const { return m_buffer[row * ncol + col]; }
        double& operator()(size_t row, size_t col) { return m_buffer[row * ncol + col]; }

        size_t get_nrow() const { return nrow; }
        size_t get_ncol() const { return ncol; }


        bool operator==(const Matrix& rhs) const {
                for (size_t row = 0; row < nrow; row++) {
                        for (size_t col = 0; col < ncol; col++) {
                                if ((*this)(row, col) != rhs(row, col)) {
                                        return false;
                                }
                        }
                }
                return true;
        }
};

Matrix multiply_naive(Matrix& mat_1, Matrix& mat_2) {
	assert (mat_1.nrow == mat_2.ncol);

        Matrix res(mat_1.nrow, mat_2.ncol);
        for (size_t row = 0; row < mat_1.nrow; row++) {
                for (size_t col = 0; col < mat_2.ncol; col++) {
                        for (size_t i = 0; i < mat_1.ncol; i++) {
                                res(row, col) += mat_1(row, i) * mat_2(i, col);
                        }
                }
        }
        return res;
}

Matrix multiply_tile(Matrix& mat_1, Matrix& mat_2, size_t block_size) {
	assert (mat_1.nrow == mat_2.ncol);

	Matrix res(mat_1.nrow, mat_2.ncol);

// Not working code
/*
	for (size_t i0 = 0; i0 < mat_1.nrow; i0 += block_size) {
        	size_t imax = i0 + block_size > mat_1.nrow ? mat_1.nrow : i0 + block_size;
        	for (size_t j0 = 0; j0 < mat_2.ncol; j0 += block_size) {
            		size_t jmax = j0 + block_size > mat_2.ncol ? mat_2.ncol : j0 + block_size;
			for (size_t i = 0; i < mat_1.ncol; i++) {
				for (size_t row = i0; i0 < imax; row++) {
                                        for (size_t col = j0; j0 < jmax; col++) {
                                                res(row, col) += mat_1(row, i) * mat_2(i, col);
                        }
                    }
                }
            }
        }
*/
	return res;
}

Matrix multiply_mkl(Matrix& mat_1, Matrix& mat_2) {
        assert (mat_1.nrow == mat_2.ncol);

        double* C = new double[mat_1.nrow * mat_2.ncol];

        int m = mat_1.nrow;
        int k = mat_1.ncol;
        int n = mat_2.ncol;
        double alpha = 1.0, beta = 0.0;

        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                                m, n, k, alpha, mat_1.data(), k, mat_2.data(), n, beta, C, n);

        Matrix res(mat_1.nrow, mat_2.ncol);
        memcpy(res.data(), C, res.get_nrow() * res.get_ncol() * sizeof(double));

        return res;
}

PYBIND11_MODULE(_matrix, m) {
        m.def("multiply_naive", &multiply_naive, "naive matrix-matrix multiplication");
        m.def("multiply_tile", &multiply_tile, "matrix-matrix multiplication with tiling");
        m.def("multiply_mkl", &multiply_mkl, "uses DGEMM for matrix-matrix multiplication");
	m.def("bytes", &bytes, "bytes used count");
	m.def("allocated", &allocated, "bytes allocated for the data buffer");
 	m.def("deallocated", &deallocated, "bytes deallocated for the data buffer");
    
        py::class_<Matrix>(m, "Matrix")
                .def(pybind11::init<size_t, size_t>())
                .def("__getitem__", [](Matrix& mat, std::pair<size_t, size_t> index) {
                        return mat(index.first, index.second);
                })
                .def("__setitem__", [](Matrix& mat, std::pair<size_t, size_t> index, double val) {
                        mat(index.first, index.second) = val;
                })
                .def(py::self == py::self)
                .def_readonly("nrow", &Matrix::nrow)
                .def_readonly("ncol", &Matrix::ncol);
}
