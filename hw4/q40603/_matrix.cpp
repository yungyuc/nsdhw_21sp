#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>
#include <mkl.h>
#include <vector>

namespace py = pybind11;

struct ByteCounterImpl
{

    size_t allocated = 0;
    size_t deallocated = 0;
    size_t refcount = 0;

}; /* end struct ByteCounterImpl */

bool cmpd(double A, double B, float epsilon = 0.005f)
{
    return (fabs(A - B) < epsilon);
}

class ByteCounter
{

public:

    ByteCounter()
      : m_impl(new ByteCounterImpl)
    { incref(); }

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

/**
 * Very simple allocator that counts the number of bytes allocated through it.
 *
 * It's made to demonstrate the STL allocator and only works in this example.
 * A lot of modification is needed to use it in a real application.
 */
template <class T>
struct MyAllocator
{

    using value_type = T;

    // Just use the default constructor of ByteCounter for the data member
    // "counter".
    MyAllocator() = default;

    template <class U> constexpr
    MyAllocator(const MyAllocator<U> & other) noexcept : counter(other.counter) {}

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

MyAllocator<double> allocator;

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol), m_buffer(allocator){
        size_t nelement = nrow * ncol;
        m_buffer.resize(nelement, 0);
    }

    // No bound check.
    double   operator() (size_t row, size_t col) const { return m_buffer[row*m_ncol + col]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[row*m_ncol + col]; }


    bool operator==(Matrix const &other) {
        if (this == &other) {
            return true;
        }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) {
            return false;
        }
        for (size_t i = 0; i < m_nrow; ++i) {
            for (size_t j = 0; j < m_ncol; ++j) {
                if (! cmpd((*this)(i,j), other(i,j))) {
                    return false;
                }
            }
        }
        return true;
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }


    size_t m_nrow;
    size_t m_ncol;
    std::vector<double, MyAllocator<double>> m_buffer;

};

Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2) {
    size_t nr = mat1.nrow();
    size_t nc = mat2.ncol();
    Matrix ans(nr, nc);
    for (size_t i = 0; i<nr; i++) {
        for (size_t k = 0; k < nc; k++) {
            double sum = 0;
            for (size_t j = 0; j < mat1.ncol(); j++) {
                sum += mat1(i, j) * mat2(j, k);
            }
            ans(i, k) = sum;
        }
    }
    return ans;
}

Matrix multiply_tile(Matrix &mat1, Matrix &mat2, size_t tile_size) {
  Matrix mat3(mat1.nrow(), mat2.ncol());

  for (size_t i = 0; i < mat1.nrow(); i += tile_size) {
    for (size_t j = 0; j < mat2.ncol(); j += tile_size) {

      size_t start = mat1.nrow() < i + tile_size ? mat1.nrow() : i + tile_size;
      size_t end = mat2.ncol() < j + tile_size ? mat2.ncol() : j + tile_size;

      for (size_t k = 0; k < mat1.ncol(); k += tile_size) {
        size_t ret_edge = mat1.ncol() < k + tile_size ? mat1.ncol() : k + tile_size;
        for (size_t kk = k; kk < ret_edge; kk++) {
          for (size_t ii = i; ii < start; ii++) {
            for (size_t jj = j; jj < end; jj++) {
              mat3(ii, jj) += mat1(ii, kk) * mat2(kk, jj);
            }
          }
        }
      }
    }
  }
  return mat3;
}

Matrix * multiply_mkl(const Matrix & mat1, const Matrix & mat2)
{
	mkl_set_num_threads(1);

	Matrix * ret = new Matrix(mat1.nrow(), mat2.ncol());

	cblas_dgemm(
		CblasRowMajor,
		CblasNoTrans,
		CblasNoTrans,
		mat1.nrow(),
		mat2.ncol(),
		mat1.ncol(),
		1.0,
		mat1.m_buffer.data(),
		mat1.ncol(),
		mat2.m_buffer.data(),
		mat2.ncol(),
		0.0,
		(*ret).m_buffer.data(),
		(*ret).ncol()
		);
	return ret;
}

size_t bytes() { return allocator.counter.bytes(); }
size_t allocated() { return allocator.counter.allocated(); }
size_t deallocated() { return allocator.counter.deallocated(); }

PYBIND11_MODULE(_matrix, m) {
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("bytes", & bytes);
	m.def("allocated", & allocated);
	m.def("deallocated", & deallocated);
    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
        .def(py::init<size_t, size_t>())
        .def_property("nrow", &Matrix::nrow, nullptr)
        .def_property("ncol", &Matrix::ncol, nullptr)
        .def("__eq__", &Matrix::operator==)
        .def("__setitem__", [](Matrix &mat, std::pair<size_t, size_t> i, double v) {
            mat(i.first, i.second) = v;
        })
        .def("__getitem__", [](Matrix &mat, std::pair<size_t, size_t> i) {
            return mat(i.first, i.second);
        });
}